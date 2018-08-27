import {default as http} from 'http'
import {default as zmq} from 'zeromq'
import {default as socketio} from 'socket.io'
import {Mapped} from './utils'

export interface BridgePublisher {
	url: string
	socket: zmq.Socket
	clients: Array<SocketIO.Socket>
}

export interface BridgeRouter {
	url: string
	socket: zmq.Socket
	client: SocketIO.Socket
}

export interface BridgeEventNotification<TContent> {
	url: string
	content: TContent
}

export interface BridgeEventSubscribed {
	url: string
	success: boolean
}

const logEventDebug = (eventName: string, ...message: Array<{}>) =>
	console.log('\u001b[0;94m' + eventName + '\u001b[0m', ...message)

export class Bridge {
	private _socketioServer: SocketIO.Server;
	private _publishers: Array<BridgePublisher> = []
	private _routers: Array<BridgeRouter> = []

	public constructor(httpServer: http.Server) {
		this._socketioServer = socketio(httpServer)
		this._socketioServer.on('connection', client => {
			logEventDebug('client connected', client.id)
			client.on('disconnect', () => {
				logEventDebug('client disconnected', client.id)
				this._unsubscribeAll(client)
			})
			client.on('publisher-subscribe', ({url}: {url: string}) => this._subscribePublisher(url, client))
			client.on('publisher-unsubscribe', ({url}: {url: string}) => this._unsubscribePublisher(url, client))
			client.on('router-subscribe', ({url}: {url: string}) => this._subscribeRouter(url, client))
			client.on('router-unsubscribe', ({url}: {url: string}) => this._unsubscribeRouter(url, client))
			client.on('router-identity-set', ({url}: {url: string}) => this._identifyRouter(url, client))
		})
	}

	private _unsubscribeAll(client: SocketIO.Socket) {
		this._publishers.filter(publisher => ~publisher.clients.indexOf(client))
			.forEach(publisher => this._publisherDropClient(publisher, client))
		this._routers.filter(router => router.client === client)
			.forEach(router => this._dropRouterBridge(router))
	}

	private _subscribePublisher(url: string, client: SocketIO.Socket) {
		const confirm = (success: boolean) => this._confirmSubscription('publisher-subscribe', client, url, success)
		let publisher = this._publishers.find(publisher => publisher.url === url)
		if (publisher === undefined) {
			try {
				this._publishers.push(publisher = this._createPublishBridge(url, client))
			} catch (e) {
				console.warn('could not connect', url, ':', e)
				return confirm(false)
			}
		}
		confirm(true)
	}

	private _unsubscribePublisher(url: string, client: SocketIO.Socket) {
		const confirm = (success: boolean) => this._confirmSubscription('publisher-unsubscribe', client, url, success)
		let publisher = this._publishers.find(publisher => publisher.url === url)
		if (!publisher)
			return confirm(false)
		if (!~publisher.clients.indexOf(client))
			return confirm(false)

		this._publisherDropClient(publisher, client)
	}

	private async _subscribeRouter(url: string, client: SocketIO.Socket) {
		const confirm = (success: boolean) => this._confirmSubscription('router-subscribe', client, url, success)
		let router = this._routers.find(router => router.url === url && router.client === client)
		if (router)
			return confirm(false)
		router = await this._createRouterBridge(url, client)
		confirm(true)
	}

	private _unsubscribeRouter(url: string, client: SocketIO.Socket) {
		const confirm = (success: boolean) => this._confirmSubscription('router-unsubscribe', client, url, success)
		let router = this._routers.find(router => router.url === url && router.client === client)
		if (!router)
			return confirm(false)
		this._dropRouterBridge(router)
		confirm(true)
	}

	private _identifyRouter(url: string, client: SocketIO.Socket) {
		const confirm = (success: boolean) => this._confirmSubscription('router-identity-set', client, url, success)
		let router = this._routers.find(router => router.url === url && router.client === client)
		if (!router)
			return confirm(false)
		router.socket
		confirm(true)
	}

	private _publisherDropClient(publisher: BridgePublisher, client: SocketIO.Socket) {
		publisher.clients.splice(publisher.clients.indexOf(client))
		if (publisher.clients.length < 0) {
			this._publishers.splice(this._publishers.indexOf(publisher))
			publisher.socket.close()
			logEventDebug('destroyed empty lobby published bridge', publisher.url)
		}
	}

	private _confirmSubscription(event: string, socket: SocketIO.Socket, url: string, success: boolean) {
		const message: BridgeEventSubscribed = {url, success}
		socket.emit(event, message)
	}

	private _createPublishBridge(url: string, firstClient: SocketIO.Socket): BridgePublisher {
		logEventDebug('creating publish bridge', url)

		const socket = zmq.socket('sub')
		socket.connect(url)
		socket.subscribe('')

		const publisher: BridgePublisher = {
			url,
			socket,
			clients: [firstClient]
		}
		this._publishers.push(publisher)

		socket.on('message', message => {
			const frame: BridgeEventNotification<{}> = {
				url,
				content: JSON.parse(message.toString())
			}
			for (const client of publisher.clients)
				client.emit('notification', frame)
		})

		return publisher
	}

	private async _createRouterBridge(url: string, client: SocketIO.Socket) {
		logEventDebug('creating router bridge', client.id, url)

		const socket = zmq.socket('req')
		await new Promise((resolve) => {
			socket.connect(url)
			socket.on('connect', () => resolve())
		})

		const router: BridgeRouter = {
			url,
			socket,
			client
		}
		return router
	}

	private _dropRouterBridge(router: BridgeRouter) {
		logEventDebug('destroying router bridge', router.client.id, router.url)
		router.socket.close()
	}
}
