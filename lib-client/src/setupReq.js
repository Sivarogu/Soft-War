const zmq = require('zeromq');
const { getRandomID } = require('./helpers');
const { parseMessage } = require('./utils');

module.exports = function setup({ PROTOCOL_ROUTER,
  ADDR_ROUTER,
  PORT_ROUTER,
  RETRY,
  IDENTITY_BASE }) {
  const waitingForRep = [];

  console.log('Starting Client...');
  const requester = zmq.socket('req');
  requester.monitor(500, 0);

  let identity = `${IDENTITY_BASE}${getRandomID(10)}`;
  requester.identity = identity;

  function send(command, data = null) {
    const msg = `${command}|${data}`;
    requester.send(msg);
    return new Promise((resolve, reject) => {
      waitingForRep.push((rep) => {
        const { status, data } = parseMessage(rep);
        return (status ? resolve : reject)(data);
      });
    })
  }

  function listenForReplies() {
    requester.on("message", function(reply) {
      const callback = waitingForRep.shift();
      if (callback) {
        callback(reply.toString());
      }
    });
  }

  async function identify() {
    console.log('Identifying client to server...');
    let i = 1;
    let identified = false;
    while (!identified && i < RETRY) {
      requester.identity = identity;
      try {
        await send('identify', identity);
        identified = true;
      } catch (err) {
        identity = `${IDENTITY_BASE}${getRandomID(10)}`;
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

  requester.connect(`${PROTOCOL_ROUTER}://${ADDR_ROUTER}:${PORT_ROUTER}`);

  const done = new Promise((resolve, reject) => {
    requester.on('connect', async function(fd, ep) {
      console.log('MONITOR -- connect, endpoint:', ep);
      await identify();
      console.log(`Successfuly identified to server as ${identity}`);
      resolve();
    });
  });

  return { requester, identity, send, done };
};
