import {default as path} from 'path'
import {default as express} from 'express'

export class Server {
	private _app: express.Express

	public constructor() {
		this._app = express()
		this._app.use(express.static(path.resolve(__dirname, '../../visualizer')))
	}

	public get app() {
		return this._app
	}
}
