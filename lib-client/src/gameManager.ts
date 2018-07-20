import {setupSubscriber, SubscriberConfig} from './setupSubscriber'
import { Config } from './config'

export interface GameInfo {
  map_size: number
  game_status: any
  players: Array<any>
  energy_cells: Array<any>
}

export const getGameManager = (config: SubscriberConfig) => {
  return class GameManager {
    public const gameInfo?: GameInfo

    public constructor() {
      this.onCycle = this.onCycle.bind(this)
      this.onStart = this.onStart.bind(this)
      this.onVictory = this.onVictory.bind(this)
      this.onDeath = this.onDeath.bind(this)
      this.onGameOver = this.onGameOver.bind(this)
      const EventType = {
        0: this.onCycle,
        1: this.onStart,
        2: this.onGameOver,
        3: this.onDeath,
        4: this.onVictory,
      }
      setupSubscriber(config, EventType)
    }

    onStart() {
      console.log('Start game')
    }

    onGameOver() {
      console.log('Game over')
    }

    onCycle(gameInfo: GameInfo) {
      console.log('---cycle---')
      this.gameInfo = gameInfo
    }

    onVictory() {

    }

    onDeath() {

    }

    printCycleInfo() {
      if (this.gameInfo) {
        const { map_size, game_status, players, energy_cells } = this.gameInfo
        console.log('map_size : ', map_size)
        console.log('game_status : ', game_status)
        console.log('players : ', players)
        console.log('energy_cells : ', energy_cells)
      } else {
        console.log('Nothing yet.')
      }
    }
  }
}
