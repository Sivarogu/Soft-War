import {default as socketio} from 'socket.io-client'
import {EasyEvent} from '@wawolf/easy-event'
import {BridgeEventNotification, BridgeEventOperation} from 'zmq-websocket-bridge'

export class BridgeClient {
	private _socket: SocketIOClient.Socket
	private _nextOperationId = 1
	public onConnect = new EasyEvent<undefined>()
	public onDisconnect = new EasyEvent<undefined>()
	public onNotification = new EasyEvent<BridgeEventNotification<{}>>()
	public onPublisherSubscribe = new EasyEvent<void>()
	public onPublisherUnsubscribe = new EasyEvent<void>()
	public onRouterSubscribe = new EasyEvent<void>()
	public onRouterUnsubscribe = new EasyEvent<void>()
	public onRouterIdentitySet = new EasyEvent<void>()

	public constructor(url?: string, autoConnect: boolean = false) {
		if (url)
			this._socket = socketio.connect(url, {autoConnect})
		else
			this._socket = socketio.connect({autoConnect})

		this._socket.on('connect', () => this._onSocketConnect())
		this._socket.on('disconnect', () => this._onSocketDisconnect())
		this._socket.on('notification', (notif: BridgeEventNotification<{}>) => this._onNotification(notif))
		// this._socket.on('router-command', (command: any) => console.log('router command:', command))
	}

	public connect() {
		this._socket.connect()
	}

	public get socket() {
		return this._socket
	}

	public async nextNotification<TNotif>() {
		return await new Promise((resolve) => {
			this.onNotification.add(notif => resolve(notif as TNotif), true)
		})
	}

	public async publisherSubscribe(url: string) {
		await this._operate('publisher-subscribe', {url}, () => {})
	}

	public async publisherUnsubscribe(url: string) {
		await this._operate('publisher-unsubscribe', {url}, () => {})
	}

	public async routerSubscribe(url: string) {
		await this._operate('router-subscribe', {url}, () => {})
	}

	public async routerUnsubscribe(url: string) {
		await this._operate('router-unsubscribe', {url}, () => {})
	}

	public async publisherIdentitySet(url: string, identity: string) {
		await this._operate('publisher-subscribe', {url, identity}, () => {})
	}

	public async routerSendCommand<TRet>(url: string, frame: string) {
		return await this._operate('router-command', {url, frame}, (ret) => ret as TRet)
	}

	private _genOperationId() {
		const nextId = this._nextOperationId
		this._nextOperationId++
		return nextId
	}

	private async _operate<TData extends {url: string}, TRet, TOpRet>(eventName: string, data: TData, handler: (data: TRet, op: BridgeEventOperation<TRet>) => (Promise<TOpRet> | TOpRet)) {
		const operationId = this._genOperationId()
		return await new Promise<TOpRet>((resolve, reject) => {
			const opHandler = (op: BridgeEventOperation<TRet>) => {
				if (op.operationId !== operationId)
					return
				this._socket.removeEventListener(eventName, opHandler)
				if (op.success === false)
					return reject(op.error)
				const handlerRet = handler(op.data, op)
				if (handlerRet instanceof Promise)
					handlerRet.then(resolve).catch(reject)
				else
					resolve(handlerRet)
			}

			this._socket.emit(eventName, {
				...(data as {}),
				operationId
			})
			this._socket.on(eventName, opHandler)
		})
	}

	private _onSocketConnect() {
		this.onConnect.trigger(undefined)
	}

	private _onSocketDisconnect() {
		this.onDisconnect.trigger(undefined)
	}

	private _onNotification(notification: BridgeEventNotification<{}>) {
		console.log('got notif')
		this.onNotification.trigger(notification)
	}
}
