const setup = require('./setup');

const {
  // requester,
  // identity,
  send,
  done,
} = setup();

const Signals = require('./signals')(send);

module.exports = done.then(() => Signals);
