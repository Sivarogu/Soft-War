import {default as socketio} from 'socket.io-client'
import {EasyEvent} from '@wawolf/easy-event'
import { BridgeClient } from './bridge';

export type Notification = {
	type: NotificationType
} & (CycleNotification | GameStartedNotification | GameFinishedNotification | ClientDeadNotification | ClientWinNotification)

export enum NotificationType {
	cycle_info = 0,
	game_started = 1,
	game_finished = 2,
	client_dead = 3,
	client_win = 4
}

export interface CycleNotification {
	notification_type: NotificationType.cycle_info
	data: GameInfo
}

export interface GameStartedNotification {
	notification_type: NotificationType.game_started
}

export interface GameFinishedNotification {
	notification_type: NotificationType.game_finished
}

export interface ClientDeadNotification {
	notification_type: NotificationType.client_dead
}

export interface ClientWinNotification {
	notification_type: NotificationType.client_win
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
		this._bridge.onNotification.add(({content}) => {
			const notification = content as Notification
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
		this._routerUrl = routerUrl
	}

	public async queryRouter(actionName: string, data: string | object | null) {
		if (!this._routerUrl)
			throw Error('not subscribed to a router')

		const frame = `${actionName}|${typeof data === 'object' ? JSON.stringify(data) : data}`
		const response = await this._bridge.routerSendCommand<string>(this._routerUrl, frame)
		const matches = response.match(/(ko|ok)\|(.*)/)
		if (!matches)
			throw Error('invalid response: ' + response)
		const [, status, responseData] = matches
		if (status === 'ko')
			throw Error(responseData || 'unknown error')
		return responseData
	}

	public async ping() {
		if ((await this.queryRouter('ping', null) !== 'pong'))
			throw Error('expected pong response')
	}

	public async nextNotification() {
		return await new Promise<Notification>((resolve) => {
			this.onNotification.add(resolve, true)
		})
	}

	public async nextCycle() {
		return await new Promise<GameInfo>((resolve) => {
			this.onCycle.add(resolve, true)
		})
	}

	public async identify() {
		if (!this._routerUrl)
			throw Error('not subscribed to a router')
		while (true) {
			try {
				const identity = `#0x${(Math.random() * 99).toFixed()}`
				await this.queryRouter('identify', identity)
				return identity
			}
			catch (e) {
				if (!(e instanceof Error) || e.message !== 'identity already exists')
					throw e
			}
		}
	}

	public async moveForward() {
		await this.queryRouter('forward', null)
	}

	public async moveBackward() {
		await this.queryRouter('backward', null)
	}

	public async moveLeft() {
		await this.queryRouter('leftfwd', null)
	}

	public async moveRight() {
		await this.queryRouter('rightfwd', null)
	}

	public async turnLeft() {
		await this.queryRouter('left', null)
	}

	public async turnRight() {
		await this.queryRouter('right', null)
	}

	public async fetchOrientation() {
		return parseInt(await this.queryRouter('looking', null)) as Direction
	}

	public async gatherEnergy() {
		await this.queryRouter('gather', null)
	}

	public async scanForward() {
		return JSON.parse(await this.queryRouter('watch', null)) as Array<string>
	}

	public async attack() {
		await this.queryRouter('attack', null)
	}

	public async fetchIdentity() {
		return await this.queryRouter('selfid', null)
	}

	public async fetchEnergy() {
		return parseInt(await this.queryRouter('selfstats', null))
	}

	public async inspect(identity: string) {
		return parseInt(await this.queryRouter('inspect', identity))
	}

	public async idle() {
		await this.queryRouter('next', null)
	}

	public async jumpForward() {
		await this.queryRouter('jump', null)
	}

	public static gameServerUrl({host, port}: NetworkInfo) {
		return `tcp://${host}:${port}`
	}
}
