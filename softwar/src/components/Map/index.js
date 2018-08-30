import React, { Component } from 'react';
import styled, { keyframes } from 'styled-components';
import {  Direction, GameStatus, NotificationType } from 'softwar-lib-client';

const animation = keyframes`
  0% {
    transform: translateY(0%);
  }
  100% {
    transform: translateY(50%);
  }
`;

const Container = styled.div`
  width: 100%;
  height: 100%;
  display: flex;
  flex-wrap: wrap;
`;

const getPlayer = (player) => {
  const { looking, x, y, energy, image } = player;
  return `
    ::after {
      position: absolute;
      display: block;
      content: " ";
      width: 100%;
      height: 70px;
      // top: 0;
      left: 0;
      bottom: 90%;
      right: 0;
      margin: 0 auto;
      background-image: url(/img/${image}-${Direction[looking]}.png);
      background-repeat: no-repeat;
      background-position: center;
      background-size: contain;
      z-index: 9;
      animation: ${animation} 200ms forwards;
      box-shadow: 0px 9px 6px -8px rgba(0, 0, 0, 0.6);
    }
  `;
}

const getEnergy = (energy) => {
  return `
    ::after {
      content: " ";
      width: 30%;
      height: 30%;
      position: absolute;
      background-color: blue;
    }
  `;
}


const EmptyCell = styled.div`
  overflow: visible;
  position: relative;
  background-color: gray;
  border: 1px solid rgba(37, 124, 48, 0.6);
  ${'' /* background-image: url('./img/grass.png'); */}
  ${'' /* background-size: 100px; */}
  &::before {
    content: " ";
    position: absolute;
    width: 100%;
    height: 100%;
    top: 0;
    left: 0;
    bottom: 0;
    right: 0;
    background-image: url('./img/grass.png');
    background-size: 100px;
    opacity: 1;
    filter: grayscale(40%) contrast(90%);;
  };
  flex: ${({ size, rowGap }) => `0 1 calc(${size}*100% - ${size % 1 ? rowGap / 2 : 0}px)`};
  flex-shrink: 0;
  ${({ player }) => player && getPlayer(player)};
  ${({ energy }) => energy && getEnergy(energy)};
`;

class Map extends Component {
  render() {
    const { size, players, energies } = this.props;
    console.log('players : ', players);
    const map = [];
    for (let i = 0; i < size; i++) {
      for (let j = 0; j < size; j++) {
        map.push(<EmptyCell player={players.find((p) => p.x === i && p.y === j)} energy={energies.find((e) => e.x === i && e.y === j)} key={`x=${i},y=${j}`} rowGap={0} size={1 / 5} />)
      }
    }
    return (
      <Container>
        {map}
      </Container>
    );
  }
}

export default Map;