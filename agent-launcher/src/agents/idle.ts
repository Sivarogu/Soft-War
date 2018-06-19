import {Agent} from '../agent'
import {ServerInfo} from '../launcher'

// TODO: make a sample idle ia which connects to a server, and wait for the game to end without doing anything

export class Idle extends Agent {
	public onConnect = async ({host, port}: ServerInfo) =>
		console.log('idle agent connected to', `${host}:${port}`)
}

export default Idle
