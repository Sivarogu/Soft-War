// TODO: make a script which accepts as parameter server connection information, and a list of agents names in src/agents to attach to it.
import * as softwar from '@ltdlragda/softwar-lib-client'
import { Agent } from './agent';

export interface ServerInfo {
	host: string
	port: string
}

export type IAHandler = (info: {serverInfo: ServerInfo}) => Promise<void>

// check argv
if (process.argv.length !== 2) {
	console.log('Usage: launcher.js {server: {host: string, port: string}, agents: Array<string>}')
	process.exit(1)
}

// load config from argv[1]
console.log('loading config from argv')
const config: {server: ServerInfo, agents: Array<string>} = JSON.parse(process.argv[1])

/* TODO: for now, we simply run the agents, but the goal is to use the lib-client to connect to the configured server,
	check for available slots, and then instanciate and connect the agents to this server */
config.agents.map(name => {
	const agentClass: new () => Agent = require(`agents/${name}`)
	const agent = new agentClass()
})
