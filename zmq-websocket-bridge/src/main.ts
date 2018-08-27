import {default as http} from 'http'
import {Server} from './server'
import {Bridge, BridgeEventPublication, BridgeEventSubscribedPublisher} from './bridge'

export {BridgeEventPublication, BridgeEventSubscribedPublisher}

const appServer = new Server()
const httpServer = new http.Server(appServer.app)
const bridge = new Bridge(httpServer);

const port = process.env.PORT ? parseInt(process.env.PORT) : 9127

console.debug("launching server...")
httpServer.listen(port, () => console.log('server listening on port', port))
