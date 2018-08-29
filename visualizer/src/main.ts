import './main.scss'
import {default as $} from 'jquery'
import {SoftwarAPI, NotificationType} from 'softwar-lib-client'

$(() => {
	const $canvas = $('#game_canvas')
	const $networkStatus = $('#game_network_status')
	const $players = $('#game_player_status')
	$networkStatus.text('Socket.io connecting...')

	const api = new SoftwarAPI()
	api.onConnect.add(async () => {
		$canvas.text('Socket.io connected')
		await api.subscribePublisher({host: 'localhost', port: 4243})
		console.log('succesfully joined game server')
		$networkStatus.html('Connected with game server <b>tcp://localhost:4243</b>')
	})

	api.onNotification.add(notification => {
		const date = (new Date()).toString()
		atestr = date.split(' GMT')[0]
		console.log(`[${datestr}] notification:`, notification)
		$canvas.text('').append(
			$('<span>').text(`Last notification - last updated ${datestr}:`),
			$('<pre>').text('notification: ' + JSON.stringify(notification, null, 4))
		)
	})

	api.onCycle.add(gameInfo => {
		if (gameInfo.players.length)
			$players.empty().append(...gameInfo.players.map(player =>
				$('<pre>').text(JSON.stringify(player))
			))
		else
			$players.text('Lobby is empty')
	})

	api.onDisconnect.add(() => {
		$networkStatus.text('Socket.io disconnected')
	})

	api.connect()

	$('#game_add_ai').click(() => {
		console.warn('not available')
	})

	; (window as any).api = api
})
