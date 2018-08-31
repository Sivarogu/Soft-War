import { injectGlobal } from 'styled-components';
import theme from './theme';
import LightPixel from './pixel.ttf';

injectGlobal([`

  @font-face {
    font-family: 'Light Pixel';
    src: url(${LightPixel}) format('woff');
  }

  @import url('https://fonts.googleapis.com/css?family=Pangolin|Schoolbell');
  @import url('https://fonts.googleapis.com/css?family=Montserrat');


  *, *:before, *:after {
    margin: 0;
    padding: 0;
    color: inherit;
    font: inherit;
    line-height: inherit;
    text-transform: inherit;
    text-decoration: inherit;
    // text-align: inherit;
    text-indent: inherit;
    text-transform: inherit;
    letter-spacing: inherit;
    box-sizing: inherit;
    opacity: inherit;
  }

  html {
    box-sizing: border-box;
    line-height: 1.15;
    -webkit-text-size-adjust: 100%;
    height: 100%;
    width: 100%;
  }

  h1 {
    font-size: 2em;
    margin: 0.67em 0;
  }

  hr {
    box-sizing: content-box;
    height: 0;
    overflow: visible;
  }

  pre {
    font-family: monospace, monospace;
    font-size: 1em;
  }

  a {
    background-color: transparent;
  }

  abbr[title] {
    border-bottom: none;
    text-decoration: underline;
    text-decoration: underline dotted;
  }

  b,
  strong {
    font-weight: bolder;
  }

  code,
  kbd,
  samp {
    font-family: monospace, monospace;
    font-size: 1em;
  }

  small {
    font-size: 80%;
  }

  sub,
  sup {
    font-size: 75%;
    line-height: 0;
    position: relative;
    vertical-align: baseline;
  }

  sub {
    bottom: -0.25em;
  }

  sup {
    top: -0.5em;
  }

  img {
    border-style: none;
  }

  button,
  input,
  optgroup,
  select,
  textarea {
    font-family: inherit;
    font-size: 100%;
    line-height: 1.15;
    margin: 0;
  }

  button,
  input {
    overflow: visible;
  }

  button,
  select {
    text-transform: none;
  }

  button,
  [type="button"],
  [type="reset"],
  [type="submit"],
  input[type="file" i]::-webkit-file-upload-button {
    -webkit-appearance: button;
  }

  button::-moz-focus-inner,
  [type="button"]::-moz-focus-inner,
  [type="reset"]::-moz-focus-inner,
  [type="submit"]::-moz-focus-inner {
    border-style: none;
    padding: 0;
  }

  button:-moz-focusring,
  [type="button"]:-moz-focusring,
  [type="reset"]:-moz-focusring,
  [type="submit"]:-moz-focusring {
    outline: 1px dotted ButtonText;
  }

  fieldset {
    padding: 0.35em 0.75em 0.625em;
  }

  legend {
    box-sizing: border-box;
    color: inherit;
    display: table;
    max-width: 100%;
    padding: 0; /* 3 */
    white-space: normal;
  }

  progress {
    vertical-align: baseline;
  }

  textarea {
    overflow: auto;
  }

  // remove autocomplete background color
  input:-webkit-autofill,
  textarea:-webkit-autofill,
  select:-webkit-autofill {
    transition: background-color 5000s ease-in-out 0s !important;
    -webkit-box-shadow: 0 0 0 0 white inset !important;
  }

  [type="checkbox"],
  [type="radio"] {
    box-sizing: border-box;
    padding: 0;
  }

  [type="number"]::-webkit-inner-spin-button,
  [type="number"]::-webkit-outer-spin-button {
    height: auto;
  }

  [type="search"] {
    -webkit-appearance: textfield;
    outline-offset: -2px;
  }

  [type="search"]::-webkit-search-decoration {
    -webkit-appearance: none;
  }

  ::-webkit-file-upload-button {
    -webkit-appearance: button;
    font: inherit;
  }

  details {
    display: block;
  }

  summary {
    display: list-item;
  }

  template {
    display: none;
  }

  [hidden] {
    display: none;
  }

  body, div, * {
    box-sozo
  }

  body {
    margin: 0;
    padding: 0;
    background: ${theme.color.darkBlue};
    font-family: ${theme.fontFamily};
    color: ${theme.color.lightGrey};
    height: 100%;
    width: 100%;
  }

  #root {
    width: 100%;
    height: 100%;
    min-width: 100%;
    min-height: 100%;
  }

  ::-webkit-scrollbar:active {
    width: 10px;
    height: 10px;
  }

  ::-webkit-scrollbar-track {
    -webkit-box-shadow: inset 0 0 3px rgba(0,0,0,0.3);
    border-radius: ${theme.radius.m};
  }

  ::-webkit-scrollbar-thumb {
    -webkit-box-shadow: inset 0 0 3px rgba(0,0,0,0.3);
    border-radius: ${theme.radius.m};
  }

  div {
    ::-webkit-scrollbar {
      width: 10px;
      height: 10px;
    }
    ::-webkit-scrollbar-track {
      background-color: 'transparent';
      border: 9px solid transparent;
    }
    ::-webkit-scrollbar-thumb {
      background: rgb(227, 227, 227, 0.5);
    }
  }

  a:-webkit-any-link {
    text-decoration: none;
  }
`]);
