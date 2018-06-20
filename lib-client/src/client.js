const com = require('./com');
const config = require('./config');

const { Manager, connected } = com(config)
// extend Manager to have cycle and game start event or use as is
// example
const manager = new Manager();

connected.then(function main (Signals) {
  // do stuff here with Signals
  // example
  Signals.watch()
    .then((rep) => {
      console.log('watch !', rep);
    })
    .catch((err) => {
      console.log('err');
    });

  Signals.selfstats()
    .then((rep) => {
      console.log('selfstats !', rep);
    })
    .catch((err) => {
      console.log('err');
    });
});
