const com = require('./com');

com.then(function main (Signals) {
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
