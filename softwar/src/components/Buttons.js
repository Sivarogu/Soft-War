import React from 'react';
import styled from 'styled-components';

export const Button = styled.div`
  width: ${({width}) => width};
  ${'' /* font-family: ${({theme}) => theme.font.button}; */}
  font-family: 'Montserrat';
  text-shadow: 2px 2px 8px #FF0000;
  background: ${({theme}) => theme.gradient.purple};
  border-radius: 20px;
  color: black;
  text-align: center;
  padding: 20px;
  font-weight: ${({bold}) => bold ? 'bold' : ''};
  margin: 10px 0;
  border: 1px solid white;
  box-shadow: 1px 1px 0px 3px black;
  font-size: 20px;
  &:hover, &:hover div {
    color: white;
  }
  &:active, &:active div {
    text-shadow: 2px 2px 8px #ffb000;
    color: black;
    background: ${({theme}) => theme.gradient.purpleReversed};
  }
`;

export const PolygonButton = styled(Button)`
  box-shadow: 1px 1px 0px 5px black;
  margin: 5px;
  height: ${({height}) => height};
  border-radius: 10px;
  background: ${({theme, color}) => theme.gradient[color]};
  border: 3px solid white;
  position: relative;
`;

const LeftActionButtonBackground = styled(PolygonButton)`
  transform: skewX(20deg);
`;

const RightActionButtonBackground = styled(PolygonButton)`
  transform: skewX(-20deg);
`;

const ActionButtonContent = styled.div`
  font-family: ${({theme}) => theme.font.button};
  text-shadow: 1px 1px 2px #aaaaaa;
  position: absolute;
  width: 100%;
  height: 100%;
  text-align: center;
  top: 0;
  display: flex;
  color: ${({theme}) => theme.color.black};
  align-items: center;
  justify-content: center;
`;

const ActionButtonContainer = styled.div`
  position: relative;
  z-index: 1;
  &:hover ${ActionButtonContent} {
    color: white;
  };
  &:active ${ActionButtonContent} {
    color: black;
  };
  &:hover ${RightActionButtonBackground},
  &:hover ${LeftActionButtonBackground} {
    filter: brightness(110%);
  };
  &:active ${RightActionButtonBackground},
  &:active ${LeftActionButtonBackground} {
    background: ${({theme}) => theme.color.brightYellow};
  };
`;

export const ActionButton = ({children, right, width, height, color, onClick, className}) => {
  return (
    <ActionButtonContainer className={className} onClick={onClick} color={color}>
      { right ? <RightActionButtonBackground width={width} height={height} color={color} /> : <LeftActionButtonBackground className="action_button_background" width={width} height={height} color={color} /> }
      <ActionButtonContent
        className="action_button_content"
        color={color}>
        {children}
      </ActionButtonContent>
    </ActionButtonContainer>
  );
};
