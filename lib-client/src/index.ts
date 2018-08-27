import {default as socketio} from 'socket.io-client'
import {EasyEvent} from '@wawolf/easy-event'
import {BridgeEventPublication, BridgeEventSubscribedPublisher} from 'zmq-websocket-bridge'

export type Notification = CycleNotification

export enum NotificationType {
	cycle_info = 0,
	game_started = 1,
	game_finished = 2,
	client_dead = 3,
	client_win = 4
}

export interface CycleNotification {
	type: NotificationType.cycle_info
	data: GameInfo
}

export enum GameStatus {
	pending = 0,
	started = 1,
	finished = 2
}

export interface GameInfo {
	map_size: number
	game_status: GameStatus
	players: Array<Player>
	energy_cells: Array<EnergyCell>
}

export interface Player {
	x: number
	y: number
	energy: number
	looking: Direction
}

export enum Direction {
	left = 0,
	top = 1,
	right = 2,
	bottom = 3
}

export interface EnergyCell {
	x: number
	y: number
	value: number
}

export class SoftwarAPI {
	private _socket: SocketIOClient.Socket
	public onConnect = new EasyEvent<undefined>()
	public onDisconnect = new EasyEvent<undefined>()
	public onNotification = new EasyEvent<Notification>()
	public onCycle = new EasyEvent<GameInfo>()

	public constructor(url?: string, autoConnect: boolean = false) {
		if (url)
			this._socket = socketio.connect(url, {autoConnect})
		else
			this._socket = socketio.connect({autoConnect})

		this._socket.on('connect', () => console.log('socket connected') || this.onConnect.trigger(undefined))
		this._socket.on('disconnect', () => this.onDisconnect.trigger(undefined))
		this._socket.on('notification', ({notification, data: gameInfo}: BridgeEventPublication<Notification>) => {
			this.onNotification.trigger(notification)
			switch (notification.notification_type) {
				case NotificationType.cycle_info:
					this.onCycle.trigger(notification.data)
					break
			}
		})
	}

	public connect() {
		this._socket.connect()
	}

	public get socket() {
		return this._socket
	}

	public joinGameServer = async (network: {host: string, port: number}) => {
		this._socket.emit('listen-gameserver', network)
		return await new Promise((resolve, reject) => {
			this._socket.once('listening-gameserver', ({url, success}: BridgeEventSubscribedPublisher) => {
				if (success)
					return resolve(url)
				reject('connexion failed')
			})
		})
	}

	public async sendCommand<TRet>(name: string, data: {}) {
		this._socket.emit('request', {name, data})
		return await new Promise((resolve, reject) => {
			this._socket.once('response', responseStatus => {
				resolve(responseStatus)
			})
		})
	}

	public identify() {

	}
}
