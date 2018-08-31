import React, { Component } from 'react';
import styled, { keyframes } from 'styled-components';
import {  Direction } from 'softwar-lib-client';

const bounce = keyframes`
  0% {
    transform: translateY(0%);
  }
  50% {
    transform: translateY(20%);
  }
  100% {
    transform: translateY(0%);
  }
`;

const float = keyframes`
  0% {
    transform: translateY(0%);
  }
  50% {
    transform: translateY(20%);

  }
  100% {
    transform: translateY(0%);
  }
`;

const Container = styled.div`
  width: 100%;
  height: 100%;
  display: flex;
  flex-wrap: wrap;
  box-shadow: 9px 6px 8px rgba(0, 0, 0, 0.6);

`;

const getAnimation = () => {
  return `
    ::after {
      animation: ${bounce} 100ms linear;
    }
  `;
};

const getPlayer = (player) => {
  const { looking, image } = player;
  return `
    ::after {
      position: absolute;
      display: block;
      content: " ";
      width: 100%;
      height: 70px;
      left: 0;
      bottom: 0;
      right: 0;
      margin: 0 auto;
      background-image: url(/img/${image}-${Direction[looking]}.png);
      background-repeat: no-repeat;
      background-position: center;
      background-size: contain;
      z-index: 9;
    }
  `;
}

const getEnergy = (energy) => {
  return `
    ::after {
      content: " ";
      top: 0;
      left:0;
      bottom: 0;
      right: 0;
      width: 50%;
      height: 50%;
      background-image: url(/img/apple.png);
      background-repeat: no-repeat;
      background-position: center;
      background-size: contain;
      position: absolute;
      animation: ${float} 1s infinite;
    }
  `;
}


const EmptyCell = styled.div`
  overflow: visible;
  position: relative;
  background-color: gray;
  border: 1px solid rgba(37, 124, 48, 0.6);
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
  ${({ player, energy }) => !player && energy && getEnergy(energy)};
  ${({ player }) => player && getPlayer(player)};
  ${({ player, energy }) => player && energy && getAnimation()};
`;

class Map extends Component {

  render() {
    const { size, players, energies } = this.props;
    console.log('Map players : ', players);

    const map = [];
    for (let i = 0; i < size; i++) {
      for (let j = 0; j < size; j++) {
        map.push(<EmptyCell player={players.find((p) => !p.dead && p.x === i && p.y === j)} energy={energies.find((e) => e.x === i && e.y === j)} key={`x=${i},y=${j}`} rowGap={0} size={1 / size} />)
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