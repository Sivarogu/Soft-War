import React from 'react';
import ReactDOM from 'react-dom';
import App from './App';
import './global-styles';
import { ThemeProvider } from 'styled-components';
import theme from './theme.js';

ReactDOM.render(
 ( <ThemeProvider theme={theme}>
  <App />
  </ThemeProvider>),
  document.getElementById('root'),
);

if (module.hot) {
  module.hot.accept('./App', () => {
    ReactDOM.render(
      (<ThemeProvider theme={theme}>
        <App />
        </ThemeProvider>),
      document.getElementById('root'),
    );
  });
}
