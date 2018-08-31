import React, { Component } from 'react';
import styled from 'styled-components';

const Container = styled.div`
  position: relative;
  width: 300px;
  max-height: 400px;
  display: flex;
  align-items: center;
  justify-content: center;
  flex-direction: column;
`;

const Player = styled.div`
  width: 100%;
  display: flex;
  align-items: center;
  justify-content: center;
  flex-direction: row;
`;

const NameWrapper = styled.div`
  display: flex;
  align-items: center;
  justify-content: center;
  flex-direction: column;
  margin-right: 20px;
  width: 88px;
`;

const Img = styled.div`
  width: 50px;
  height: 50px;
  background-image: ${({src}) => `url(/img/head-${src}-top.png)`};
  background-repeat: no-repeat;
  background-position: center;
  background-size: cover;
`;

const MAX = 50;

const Energy = styled.div`
  position: relative;
  flex: 1;
  width: 100%;
  height: 40px;
  border: 1px solid black;
  box-shadow: 1px 1px 5px rgba(0,0,0 0.5);
  &::after {
    content: " ";
    width: ${({ val }) => (val * 100 / MAX)}%;
    background-color: ${({ theme }) => theme.color.green};
    height: 100%;
    position: absolute;
    top: 0;
    bottom: 0;
    left: 0;
  }
`;

const Name = styled.div`
  color: white;
  font-size: 25px;
  margin: 5px;
  font-family: 'Montserrat';
`;

class PlayerDisplayer extends Component {
  render() {
    const { players } = this.props;
    console.log('player displayer players : ', players);
    return (
      <Container>
        {players.map(p => (
          <Player key={p.name}>
            <NameWrapper>
              <Img src={p.image} />
              <Name>{p.name}</Name>
            </NameWrapper>
            <Energy val={p.energy} />
          </Player>
        ))}
      </Container>
    );
  }
}

export default PlayerDisplayer;