import './main.scss'
import {default as $} from 'jquery'
import {SoftwarAPI, GameInfo, Player, EnergyCell} from 'softwar-lib-client'

const routerUrl = {host: 'softwar-server', port: 4242}
const publisherUrl = {host: 'softwar-server', port: 4243}

$(() => {
	const $canvas = $('#game_canvas')
	const $networkStatus = $('#game_network_status')
	const $players = $('#game_player_status')
	$networkStatus.text('Socket.io connecting...')

	const api = new SoftwarAPI()
	api.onConnect.add(async () => {
		$canvas.text('Socket.io connected')
		await api.subscribePublisher(publisherUrl)
		await api.subscribeRouter(routerUrl)
		console.log('succesfully joined game server')
		$networkStatus.html('Connected with game server <b>tcp://localhost:4243</b>')
	})

	api.onNotification.add(notification => {
		const date = (new Date()).toString()
		const datestr = date.split(' GMT')[0]
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

	const sleep = (timeout: number) => new Promise((resolve) => setTimeout(resolve, timeout))
	const nextCycle = (api: SoftwarAPI) => new Promise<GameInfo>((resolve) => api.onCycle.add(resolve, true))
	const bravely = async <T>(p: Promise<T>) => {
		try {await p}
		catch (e) {}
	}

	const startIA = async () => {
		if (players.length >= 4)
			return console.warn('already has 4 IA')
		const player = new SoftwarAPI()

		players.push(player)

		player.connect()
		await player.subscribePublisher(publisherUrl)
		await player.subscribeRouter(routerUrl)
		const identity = await player.identify()

		let cycleInfo: GameInfo
		cycleInfo = await player.nextCycle();
		const curr_player = cycleInfo.players.find(pl => pl.name === identity);
		if (!curr_player)
			return;
		while (cycleInfo = await player.nextCycle()) {
			let action_pts: number = 2;
			let energy: EnergyCell|undefined
			if (cycleInfo.energy_cells.length > 0) {
				energy = getClosestEnergy(curr_player, cycleInfo);
				if (energy.x === curr_player.x && energy.y === curr_player.y && curr_player.energy < 100 - energy.value) {
					await bravely(player.gatherEnergy())
					action_pts -= 2;
				}
			}
			let vision = await player.scanForward()
			vision.forEach(async square => {
				if (square != "energy" && square != "empty" && curr_player.energy > 20 && action_pts > 1) {
					await bravely(player.attack())
					action_pts--;
				}
			});
			while(energy && action_pts > 0) {
				let action = goTo(curr_player, energy);
				switch (action) {
					case "forward": await bravely(player.moveForward()); break;
					case "backward": await bravely(player.moveBackward()); break;
					case "leftfwd": await bravely(player.moveLeft()); break;
					case "right": await bravely(player.turnRight()); break;
					default: break;
				}
			}
		}
	}

	const goTo = (player: Player, energy: EnergyCell):string => {
		let dist = energy.x - player.x;
		let look_dir_pos = player.looking;
		if (dist === 0) {
			dist = energy.y - player.y;
			look_dir_pos--;
		}
		if ((dist > 0 && player.looking === 2) ||
			(dist < 0 && player.looking === 0)) {
			return "forward";
		} else if ((dist === -1 && player.looking === 2) ||
			(dist === 1 && player.looking === 0)) {
			return "backward";
		} else if ((dist < 0 && player.looking === 1) ||
			(dist > 0 && ((player.looking + 4) % 4)  === 3)) {
			return "leftfwd";
		} else {
			return "right";
		}
		return "";
	}

	const getClosestEnergy = (player: Player, cycleInfo: GameInfo): EnergyCell => {
		let energy: EnergyCell = cycleInfo.energy_cells[0];
		let minDistance: number = Math.pow(cycleInfo.map_size, 2);
		cycleInfo.energy_cells.forEach(cell => {
			let distance = Math.abs(player.x - cell.x) + Math.abs(player.y - cell.y);
			if (distance > minDistance){
				minDistance = distance;
				energy = cell;
			}
		})
		return energy;
	}

	const players: Array<SoftwarAPI> = []
	const $addAIButton = $('#game_add_ai').click(() => {
		startIA()
		$addAIButton.text('Add a sample AI (' + (4 - players.length) + ' remaining)')
		if (players.length >= 4) {
			$addAIButton.remove()
			$addAISButton.remove()
		}
	})
	const $addAISButton = $('#game_add_ais').click(() => {
		for (let i = players.length; i < 4; i++)
			$addAIButton.click()
	})

	; (window as any).api = api
	; (window as any).players = players
})
