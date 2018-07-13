const zmq = require('zeromq');
const { parseNotif } = require('./utils');

module.exports = function({ PROTOCOL_PUB,
  ADDR_PUB,
  PORT_PUB,
  identity }, Events) {
  console.log('Starting sub...');
  const subscriber = zmq.socket('sub');
  if (identity) {
    subscriber.identity = identity;
    // subscriber.subscribe(identity);
  }
  subscriber.on('message', function() {
    const msg = [];
    Array.prototype.slice.call(arguments).forEach(function(arg) {
      msg.push(arg.toString());
    });

    const data = parseNotif(msg[0]);
    if (data) {
      Events[data.notification_type](data.data);
    }
  });

  subscriber.connect(`${PROTOCOL_PUB}://${ADDR_PUB}:${PORT_PUB}`);
  subscriber.subscribe('');

  process.on('SIGINT', function() {
    subscriber.unsubscribe('');
    subscriber.close();
  });
};
