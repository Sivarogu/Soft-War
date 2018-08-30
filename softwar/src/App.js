import React, { Component } from 'react';
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
  width: 250px;
  height: 250px;
  overflow: visible;
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
  font-family: 'Light Pixel';
  font-size: 8vw;
  width: 40%;
`;

const Notification = styled.div`

`;

const IMAGES = ["cat", "black-bunny", "bunny", "pink-cat"];




const isGameStarted = (notif) => notif.game_status === GameStatus.started;

class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      connected: false,
      notification: {},
      data: {
        map_size: 5,
        energy: [],
      },
      players: MOCK_PLAYERS.map((p, i) => ({
        ...p,
        image: IMAGES[i],
        dead: false
      })),
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
    const { connected, notification, players, data } = this.state;
    return (
      <Wrapper>
        <Header>
          <H1 connected={connected}>Kawaï SoftWar</H1>
        </Header>
        <Container>
          <Content>
            {data && (
              <Map
                size={data.map_size}
                // players={notification.data.players || []}
                players={players}
                energies={data.energy || []}
              />
            )}
          </Content>
        </Container>
      </Wrapper>
    );
  }
}

export default App;
