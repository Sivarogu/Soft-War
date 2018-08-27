
export class EasyEvent<TData> {
	private _queue: Array<(data: TData) => void> = []

	public trigger(data: TData) {
		this._queue.forEach((handler) => {
			if (!handler)
				console.error('got undefined handler in foreach')
			handler(data)
		})
	}

	public add(handler: (data: TData) => void, once: boolean = true) {
		if (once)
			this._queue.push(data => {
				this.remove(handler)
				handler(data)
			})
		else
			this._queue.push(handler)
	}

	public remove(handler: (data: TData) => void) {
		const index = this._queue.indexOf(handler)
		if (index >= 0)
			this._queue.splice(index)
	}
}
