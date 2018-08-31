import React, { Component } from 'react';
import { SoftwarAPI, GameStatus as GAME_STATUS, NotificationType } from 'softwar-lib-client';
import Map from './components/Map';
import GameHandler from './components/GameHandler';
import PlayersDisplayer from './components/PlayerDisplayer';
import styled from 'styled-components';

const api = new SoftwarAPI("http://localhost:9127");
const IMAGES = ["cat", "black-bunny", "bunny", "pink-cat"];

const Wrapper = styled.div`
  width: 100%;
  height: 100%;
  display: flex;
  align-items: center;
  justify-content: flex-start;
  flex-direction: column;
  background: url('/img/background1.png') repeat;
`;

const Content = styled.div`
  width: 500px;
  height: 500px;
  overflow: visible;
`;

const Container = styled.div`
  flex: 1;
  width: 100%;
  max-width: 100%;
  height: 100%;
  display: flex;
  align-items: center;
  justify-content: space-around;
  flex-direction: row;
`;

const Header = styled.div`
  margin: 20px;
  width: 100%;
  font-size: 20pt;
  text-align: center;
  display: flex;
  align-items: center;
  justify-content: center;
  flex-direction: column;
`;

const H1 = styled.div`
  margin: 20px;
  text-align: center;
  font-family: 'Light Pixel';
  font-size: 5vw;
  height: 145px;
  width: 40%;
  color: white;
  -webkit-text-stroke-width: 1px;
  -webkit-text-stroke-color: black;
  text-shadow:
       3px 3px 0 #000,
     -1px -1px 0 #000,
      1px -1px 0 #000,
      -1px 1px 0 #000,
       1px 1px 0 #000;
`;


class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      connected: false,
      notification: {},
      mapSize: 10,
      energyCells: [],
      players: {},
      status: GAME_STATUS.pending,
    }
    api.onConnect.add(this.onConnect);
    api.onDisconnect.add(this.onDisconnect);
    api.onNotification.add(this.onNotification);
    // api.onGameStarted.add(this.onGameStart);
    // api.onGameFinished.add(this.onGameFinished);
    api.onClientDead.add(this.onClientDead);
    api.connect();
  }

  onConnect = async () => {
    console.log('CONNECTED');
    this.setState({ connected: true });
    await api.subscribePublisher({ host: 'softwar-server', port: 4243 });
    console.log('succesfully joined game server')
  }

  onClientDead = ({ target: name}) => {
    this.setState(({ players }) => {
      console.log('players : ', players);
      console.log('On CLient is dead !! = ', name, players[name]);

      return { players: { ...players, [name]: { ...players[name], dead: true } }};
    });
  }

  onNotification = (notification) => {
    const date = (new Date()).toString();
    const datestr = date.split(' GMT')[0];
    const { data, notification_type: type  } = notification;
    let i = 0;
    if (type === NotificationType.cycle_info && data) {
        i++;
        this.setState((prev) => {
          let state = {
            mapSize: data.map_size,
            status: data.game_status,
            energyCells: data.energy_cells,
          };
          let players, firstPlayers;
          if (data.energy_cells.length === 0) {
            console.log('firstplayer');
            firstPlayers = players = data.players.reduce((players, p, n) => {
            return {
              ...players,
                [p.name]: {
                  ...p,
                  image: IMAGES[n],
                  dead: false,
                }
              };
            }, {});
            state = { ...state, players, firstPlayers };
          } else {
            players = Object.values(prev.players).reduce((players, p, n) => {
              const self = data.players.find((pl) => pl.name === p.name);
              if (self) {
                return {
                  ...players,
                  [p.name]: {
                    ...p,
                    ...self
                  }
                }
              }
              return {
                ...players,
                [p.name]: {
                  ...p,
                  dead: true,
                }
              }
            }, prev.players);
            state = { ...state, players };
          }
          return (state);
        });
    }
    console.log(`[${datestr}] notification:`, notification);
  }

  updatePlayer = (name, data) => {
    console.log(`\nPLAYER${name} = `,  data);

    this.setState(({ players }) => {
      const player = players[name];
      if (!player) {
        return { players: { ...players, [name]: data } };
      }
      return { players: { ...players, [name]: { ...player, ...data } } };
    });
  }

  onDisconnect = () => {
    this.setState({ connected: false });
    console.log('DISCONNECTED');
  };

  render() {
    const {
      players,
      energyCells,
      mapSize,
      status,
    } = this.state;

    return (
        <Wrapper>
          <Header>
            <H1>Kawaï SoftWar</H1>
            <GameHandler status={status} updatePlayer={this.updatePlayer} players={players} />
          </Header>
          <Container>
            <PlayersDisplayer players={Object.values(players)} />
            <Content>
              <Map
                size={mapSize}
                players={Object.values(players)}
                energies={energyCells}
              />
            </Content>
          </Container>
        </Wrapper>
    );
  }
}

export default App;
