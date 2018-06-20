var zmq = require('zeromq');

module.exports = function (send) {
  return ({
    identify: function identify(id) {
      return send('identify', id);
    },
    forward: function forward() {
      return send('forward');
    },
    backward: function backward() {
      return send('backward');
    },
    leftfwd: function leftfwd() {
      return send('leftfwd');
    },
    rightfwd: function rightfwd() {
      return send('rightfwd');
    },
    right: function right() {
      return send('right');
    },
    left: function left() {
      return send('left');
    },
    gather: function gather() {
      return send('gather');
    },
    watch: function watch() {
      return send('watch');
    },
    left: function left() {
      return send('left');
    },
    looking: function looking() {
      return send('looking');
    },
    gather: function gather() {
      return send('gather');
    },
    watch: function watch() {
      return send('watch');
    },
    attack: function attack() {
      return send('attack');
    },
    selfid: function selfid() {
      return send('selfid');
    },
    selfstats: function selfstats() {
      return send('selfstats');
    },
    inspect: function inspect(id) {
      return send('inspect', id);
    },
    next: function next() {
      return send('next');
    },
    jump: function jump() {
      return send('jump');
    },
  });
};
