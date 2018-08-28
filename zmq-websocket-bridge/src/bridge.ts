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

export interface BridgeOperation {
	operationId: number | string
	url: string
}

export type BridgeEventOperation<TData> =
	BridgeEventOperationError | BridgeEventOperationSuccess<TData>

export interface BridgeEventOperationSuccess<TData> {
	operationId?: number | string
	url: string
	success: true
	data: TData
}

export interface BridgeEventOperationError {
	operationId: number | string
	url: string
	success: false
	error: string
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

			this._createRoute(client, 'publisher-subscribe', ({url}) => this._subscribePublisher(url, client))
			this._createRoute(client, 'publisher-unsubscribe', ({url}) => this._unsubscribePublisher(url, client))
			this._createRoute(client, 'router-subscribe', ({url}) => this._subscribeRouter(url, client))
			this._createRoute(client, 'router-unsubscribe', ({url}) => this._unsubscribeRouter(url, client))
			this._createRoute<BridgeOperation & {identity: string}>(
				client, 'router-identity-set', ({url, identity}) => this._identifyRouter(url, client, identity))
		})
	}

	private _unsubscribeAll(client: SocketIO.Socket) {
		console.log('unsubscribing all', this._publishers.filter(publisher => ~publisher.clients.indexOf(client)))
		this._publishers.filter(publisher => ~publisher.clients.indexOf(client))
			.forEach(publisher => this._publisherDropClient(publisher, client))
		this._routers.filter(router => router.client === client)
			.forEach(router => this._dropRouterBridge(router))
	}

	private _createRoute<TData extends BridgeOperation, TRet = void>(client: SocketIO.Socket, eventName: string, handler: (data: TData) => Promise<TRet>) {
		client.on(eventName, async (data: TData) => {
			try {
				const confirmation: BridgeEventOperationSuccess<TRet> = {
					url: data.url,
					operationId: data.operationId,
					success: true,
					data: await handler(data)
				}
				client.emit(eventName, confirmation)
			} catch (e) {
				const confirmation: BridgeEventOperationError = {
					url: data.url,
					operationId: data.operationId,
					success: false,
					error: String(e)
				}
				client.emit(eventName, confirmation)
			}
		})
	}

	private async _subscribePublisher(url: string, client: SocketIO.Socket) {
		let publisher = this._publishers.find(publisher => publisher.url === url)
		if (publisher) {
			if (~publisher.clients.indexOf(client))
				throw Error('already subscribed')
			publisher.clients.push(client)
		}
		else
			this._createPublishBridge(url, client)
	}

	private async _unsubscribePublisher(url: string, client: SocketIO.Socket) {
		let publisher = this._publishers.find(publisher => publisher.url === url)
		if (!publisher)
			throw Error('unknown publisher: ' + url)
		if (!~publisher.clients.indexOf(client))
			throw Error('not subscribed: ' + url)
		this._publisherDropClient(publisher, client)
	}

	private async _subscribeRouter(url: string, client: SocketIO.Socket) {
		let router = this._routers.find(router => router.url === url && router.client === client)
		if (router)
			throw Error('already subscribed: ' + url)
		await this._createRouterBridge(url, client)
	}

	private async _unsubscribeRouter(url: string, client: SocketIO.Socket) {
		let router = this._routers.find(router => router.url === url && router.client === client)
		if (!router)
			throw Error('not subscribed: ' + url)
		this._dropRouterBridge(router)
	}

	private async _identifyRouter(url: string, client: SocketIO.Socket, identity: string) {
		let router = this._routers.find(router => router.url === url && router.client === client)
		if (!router)
			throw Error('not subscribed: ' + url)
		router.socket.identity = identity
	}

	private _publisherDropClient(publisher: BridgePublisher, client: SocketIO.Socket) {
		publisher.clients.splice(publisher.clients.indexOf(client))
		if (publisher.clients.length < 0) {
			this._publishers.splice(this._publishers.indexOf(publisher))
			publisher.socket.close()
			logEventDebug('destroyed empty lobby published bridge', publisher.url)
		}
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
			const notification: BridgeEventNotification<{}> = {
				url,
				content: JSON.parse(message.toString())
			}
			for (const client of publisher.clients)
				client.emit('notification', notification)
		})

		return publisher
	}

	private async _createRouterBridge(url: string, client: SocketIO.Socket) {
		logEventDebug('creating router bridge', client.id, url)

		const socket = zmq.socket('req')
		await new Promise((resolve) => {
			socket.connect(url)
			resolve()
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
