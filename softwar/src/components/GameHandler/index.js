import React, { Component } from 'react';
import styled from 'styled-components';
import { ROUTER_URL, PUBLISHER_URL } from '../../constants';
import { SoftwarAPI, GameStatus as GAME_STATUS } from 'softwar-lib-client';
import { Button } from "../Buttons";
console.log('ROUTER_URL, ', ROUTER_URL);
console.log('PUBLISHER_URL, ', PUBLISHER_URL);

const bravely = async (p, defaultValue) => {
  try {return await p}
  catch (e) {return defaultValue}
}


const goTo = (player, energy) => {
  let dist = energy.x - player.x;
  let look_dir_pos = player.looking;
  if (dist === 0) {
      dist = energy.y - player.y;
      look_dir_pos--;
  }
  if ((dist > 0 && look_dir_pos === 2) ||
      (dist < 0 && look_dir_pos === 0)) {
      return "forward";
  } else if ((dist === -1 && look_dir_pos === 2) ||
      (dist === 1 && look_dir_pos === 0)) {
      return "backward";
  } else if ((dist < 0 && look_dir_pos === 1) ||
      (dist > 0 && ((look_dir_pos + 4) % 4)  === 3)) {
      return "leftfwd";
  } else {
      return "right";
  }
  return "";
}

const getClosestEnergy = (player, cycleInfo) => {
  let energy = cycleInfo.energy_cells[0];
  let minDistance = Math.pow(cycleInfo.map_size, 2);
  cycleInfo.energy_cells.forEach(cell => {
      let distance = Math.abs(player.x - cell.x) + Math.abs(player.y - cell.y);
      if (distance < minDistance){
          minDistance = distance;
          energy = cell;
      }
  })
  return energy;
}


class StartGame extends Component {
  state = {
    clients: [],
    totalClients: 4,
    status: 0,
  };

  startIA = async () => {
    const player = new SoftwarAPI('http://localhost:9127');

    try {
      player.connect();
      await player.subscribePublisher(PUBLISHER_URL);
      await player.subscribeRouter(ROUTER_URL);
      const identity = await player.identify();
      this.setState(({ clients: prevClients }) => ({ clients: [ ...prevClients, player ]}));

      let cycleInfo;
      while (cycleInfo = await player.nextCycle()) {
          console.log(identity, ' starting cycle');
          let curr_player = cycleInfo.players.find(pl => pl.name === identity);
          if (!curr_player)
              continue;
          let action_pts = 2;
          let energy
          if (cycleInfo.energy_cells.length > 0) {
            energy = getClosestEnergy(curr_player, cycleInfo);
            if (energy.x === curr_player.x && energy.y === curr_player.y && curr_player.energy < 100 - energy.value) {
              await bravely(player.gatherEnergy(), undefined)
              action_pts -= 2;
            }
          }
          let vision = await player.scanForward()
          await Promise.all(vision.map(async square => {
            if (square != "energy" && square != "empty" && curr_player && curr_player.energy > 20 && action_pts > 1) {
              await bravely(player.attack(), undefined)
              action_pts--;
            }
          }))
          await bravely((async () => {
            if (energy) {
              switch (goTo(curr_player, energy)) {
                case "forward": await player.moveForward(); break;
                case "backward": await player.moveBackward(); break;
                case "leftfwd": await player.moveLeft(); break;
                case "right": await player.turnRight(); break;
                default: break;
              }
            }
          })(), undefined)
      }
    } catch(err) {
      console.warn("GAME IS FULL!");
    }

  }

  startGame = () => {
    const { status } = this.props;
    if (status === GAME_STATUS.finished || status === GAME_STATUS.pending ) {
      this.setState({ clients: [] });
      for (let i = 0; i < this.state.totalClients; i++) {
        this.startIA(i);
      }
    }
  };

  render() {
    const { status }  = this.props;
    return (
      <Button
        width="180px"
        disabled={status === GAME_STATUS.started}
        green
        onClick={this.startGame}
      >
        {status === GAME_STATUS.pending && "Start Game"}
        {status === GAME_STATUS.started && "Running"}
        {status === GAME_STATUS.finished && "New Game"}
      </Button>
    );
  }
}

export default StartGame;