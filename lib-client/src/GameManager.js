const setup = require('./setupSub');

module.exports = function(config) {
  return class GameManager {
    constructor() {
      this.gameInfo = null;
      const EventType = {
        0: this.onCycle,
        1: this.onStart,
        2: this.onGameOver,
        3: this.onDeath,
        4: this.onVictory,
      };
      setup(config, EventType);
    }

    onStart() {
      console.log('Start game');
    }

    onGameOver() {
      console.log('Game over');
    }

    onCycle(gameInfo) {
      console.log('---cycle---');
      this.gameInfo = gameInfo;
    }

    onVictory() {

    }

    onDeath() {

    }

    printCycleInfo() {
      if (this.gameInfo) {
        const { map_size, game_status, players, energy_cells } = this.gameInfo;
        console.log('map_size : ', map_size);
        console.log('game_status : ', game_status);
        console.log('players : ', players);
        console.log('energy_cells : ', energy_cells);
      } else {
        console.log('Nothing yet.');
      }
    }
  };
};
