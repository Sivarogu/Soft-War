import React, { Component } from 'react';
import { SoftwarAPI } from 'softwar-lib-client';
import styled from 'styled-components';
import { ROUTER_URL, PUBLISHER_URL } from '../../constants';
import { Button } from "../Buttons";
console.log('ROUTER_URL, ', ROUTER_URL);
console.log('PUBLISHER_URL, ', PUBLISHER_URL);

const bravely = async (p, defaultValue) => {
  try {return await p}
  catch (e) {return defaultValue}
}

const IMAGES = ["cat", "black-bunny", "bunny", "pink-cat"];


class StartGame extends Component {
  state = {
    clients: [],
    totalClients: 4,
  };

  startIA = async (n) => {
    const { updatePlayer } = this.props;
    const player = new SoftwarAPI('http://localhost:9127');

    try {
      player.connect();
      await player.subscribePublisher(PUBLISHER_URL);
      await player.subscribeRouter(ROUTER_URL);
      const identity = await player.identify();
      this.setState(({ clients: prevClients }) => ({ clients: [ ...prevClients, player ]}));

      let cycleInfo;
      let i = 0;
      while (cycleInfo = await player.nextCycle()) {
        const self = cycleInfo.players.find(p => p.name === identity);
        if (self) {
          console.log(`self : ${identity} = `, self);
          let state = { ...self };
          if (!i) {
            console.log('inititalize ', identity);
            state = {
              ...state,
              image: IMAGES[n],
              dead: false,
            };
          }
          updatePlayer(identity, state);
          console.log(identity, ' starting cycle');
          await bravely(player.turnRight(), null);
          await bravely(player.attack(), null);
          await bravely(player.jumpForward(), null);
        }
        i++;
      }
    } catch(err) {
      console.warn("GAME IS FULL!");
    }

  }

  startGame = () => {
    for (let i = 0; i < this.state.totalClients; i++) {
      this.startIA(i);
    }
  };

  render() {
    return (
      <Button width="180px" green onClick={this.startGame}>Start Game</Button>
    );
  }
}

export default StartGame;