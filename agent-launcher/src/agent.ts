import {ServerInfo} from './launcher'

export abstract class Agent {
	protected onConnect?: (serverInfo: ServerInfo) => void
}
