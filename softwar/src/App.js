import React, { Component } from 'react';
import logo from './logo.svg';
import './App.css';
import { SoftwarAPI, Direction, GameStatus, NotificationType } from 'softwar-lib-client';
import Map from './components/Map';
import styled from 'styled-components';
import { MOCK_PLAYERS } from './mocks';

const api = new SoftwarAPI("http://localhost:9127");

const Wrapper = styled.div`
  width: 100%;
  height: 100%;
  display: flex;
  align-items: center;
  justify-content: flex-start;
  flex-direction: column;
`;

const Content = styled.div`
  width: 400px;
  height: 200px;
  overflow: visible;
  ${'' /* position: relative; */}
`;

const Container = styled.div`
  flex: 1;
  width: 100%;
  height: 100%;
  display: flex;
  align-items: center;
  justify-content: center;
`;

const Header = styled.div`
  width: 100%;
  background-color: darkblue;
  box-shadow: 0px 3px 10px rgba(0, 0, 0, 0.5);
`;

const IMAGES = ["cat", "cat", "cat", "cat"];




const isGameStarted = (notif) => notif.game_status === GameStatus.started;

class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      connected: false,
      notification: {},
      players: [],
    }
    api.onConnect.add(this.onConnect);
    api.onDisconnect.add(this.onDisconnect);
    api.onNotification.add(this.onNotification);
    api.connect();
  }

  onConnect = async () => {
    console.log('CONNECTED');
    this.setState({ connected: true });
    await api.subscribePublisher({ host: 'localhost', port: 4243 });
    console.log('succesfully joined game server')
  }

  onNotification = (notification) => {
    const date = (new Date()).toString();
    const datestr = date.split(' GMT')[0];
    // console.log(`[${datestr}] notification:`, notification)
    this.setState(({ notification: prev, players }) => {
      let state = { notification };
      if (!players.length) {
        state.players = MOCK_PLAYERS.map((p, i) => ({
          ...p,
          image: IMAGES[i],
          dead: false
        }));
      }
      // uncomment when api is working
      // if (!players.length &&
      //     prev.game_status === GameStatus.pending &&
      //     isGameStarted(notification)) {
      //   state.players = notification.players.map((p, i) => ({
      //     ...p,
      //     image: IMAGES[i],
      //     dead: false
      //   }));
      // }
      // else if (players.length) {
      //   state = { players: players.map(p => {
      //     const newPlayer = notification.players.find(pl => pl.name === p.name);
      //     if (newPlayer) {
      //       return {
      //         ...p,
      //         ...newPlayer,
      //       }
      //     }
      //     return {
      //       ...p,
      //       energy: 0,
      //       dead: true,
      //     }
      //   })}
      // }

      return state;
    });
  }

  onDisconnect = () => {
    this.setState({ connected: false });
    console.log('DISCONNECTED');
  };

  render() {
    const { connected, notification, players } = this.state;
    return (
      <Wrapper>
        <Header>
          <h1>{connected && 'Connected with game server tcp://localhost:4243'}</h1>
          {JSON.stringify(notification, null, 4)}
        </Header>
        <Container>
          <Content>
            {notification.data && (
              <Map
                size={notification.data.map_size}
                // players={notification.data.players || []}
                players={players}
                energies={notification.data.energy || []}
              />
            )}
          </Content>
        </Container>
      </Wrapper>
    );
  }
}

export default App;
