const zmq = require('zeromq');
const { PROTOCOL,
        ADDR,
        PORT,
        RETRY,
        IDENTITY_BASE } = require('./config');
const { getRandomID } = require('./helpers');
const { parseMessage } = require('./utils');

module.exports = function setup() {
  const waitingForRep = [];

  console.log('Starting Client...');
  const requester = zmq.socket('req');
  requester.monitor(500, 0);

  let identity = `${IDENTITY_BASE}${getRandomID(10)}`;
  requester.identity = identity;

  function send(command, data = null) {
    const msg = `${command}|${data}`;
    requester.send(msg);
    console.log('Sent: ', msg);
    return new Promise((resolve, reject) => {
      waitingForRep.push((rep) => {
        const { status, data } = parseMessage(rep);
        return (status ? resolve : reject)(data);
      });
    })
  }

  function listenForReplies() {
    requester.on("message", function(reply) {
      console.log('Received: ', reply.toString());
      const callback = waitingForRep.shift();
      if (callback) {
        callback(reply.toString());
      }
    });
  }

  async function identify() {
    console.log('Identifying client to server...');
    identity = `${IDENTITY_BASE}${getRandomID(10)}`;
    let i = 1;
    let identified = false;
    while (!identified && i < RETRY) {
      requester.identity = identity;
      try {
        await send('identify', identity);
        identified = true;
      } catch (err) {
        identified = false;
      }
    }
    if (!identified)
      throw new Error('Couldn\'t identify to server.');
  }

  listenForReplies();

  process.on('SIGINT', function() {
    requester.unmonitor();
    requester.close();
  });

  requester.connect(`${PROTOCOL}://${ADDR}:${PORT}`);

  const done = new Promise((resolve, reject) => {
    requester.on('connect', async function(fd, ep) {
      console.log('MONITOR -- connect, endpoint:', ep);
      await identify();
      resolve();
    });
  });

  return { requester, identity, send, done };
};
