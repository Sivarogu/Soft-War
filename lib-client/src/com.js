const setup = require('./setupReq');
const getGameManager = require('./GameManager');

module.exports = (config) => {
  const {
    // requester,
    identity,
    send,
    done,
  } = setup(config);
  const Signals = require('./signals')(send);
  const Manager = getGameManager({ identity, ...config });
  return {
    Manager,
    connected: done.then(() => {
      return Signals;
    }),
  };
};
