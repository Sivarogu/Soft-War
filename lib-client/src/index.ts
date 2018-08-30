import {default as socketio} from 'socket.io-client'
import {EasyEvent} from '@wawolf/easy-event'
import { BridgeClient } from './bridge';

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
	name: string
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

export interface NetworkInfo {
	host: string
	port: number
}

export class SoftwarAPI {
	private _bridge: BridgeClient
	private _publisherUrl: string|undefined
	private _routerUrl: string|undefined

	public onConnect = new EasyEvent<void>()
	public onDisconnect = new EasyEvent<void>()
	public onNotification = new EasyEvent<Notification>()
	public onCycle = new EasyEvent<GameInfo>()

	public constructor(socketUrl?: string, autoConnect: boolean = false) {
		this._bridge = new BridgeClient(socketUrl, autoConnect)

		this._bridge.onConnect.add(() => this.onConnect.trigger(undefined))
		this._bridge.onDisconnect.add(() => this.onDisconnect.trigger(undefined))
		this._bridge.onNotification.add(({content: notification}) => {
			this.onNotification.trigger(notification)
			switch (notification.notification_type) {
				case NotificationType.cycle_info:
					this.onCycle.trigger(notification.data)
					break
			}
		})
	}

	public connect() {
		this._bridge.connect()
	}

	public get socket() {
		return this._bridge.socket
	}

	public subscribePublisher = async (networkInfo: NetworkInfo) => {
		if (this._publisherUrl)
			await this._bridge.publisherUnsubscribe(this._publisherUrl)

		const publisherUrl = SoftwarAPI.gameServerUrl(networkInfo)
		await this._bridge.publisherSubscribe(publisherUrl)
		this._publisherUrl= publisherUrl
	}

	public subscribeRouter = async (networkInfo: NetworkInfo) => {
		if (this._routerUrl)
			await this._bridge.routerUnsubscribe(this._routerUrl)

		const routerUrl = SoftwarAPI.gameServerUrl(networkInfo)
		await this._bridge.routerSubscribe(routerUrl)
		this._routerUrl= routerUrl
	}

	public async queryRouter<TRet>(actionName: string, data: string) {
		if (!this._routerUrl)
			throw Error('not subscribed to a router')

		const frame = `${actionName}|${data}`
		return await this._bridge.routerSendCommand(this._routerUrl, frame)
	}

	public async identify() {
		if (!this._routerUrl)
			throw Error('not subscribed to a router')
		await this._bridge.publisherIdentitySet(this._routerUrl, 'foo')
	}

	public static gameServerUrl({host, port}: NetworkInfo) {
		return `tcp://${host}:${port}`
	}
}
