function parseMessage(message) {
  const msgArr = message.split('|', 2);
  let data = null;
  try {
    data = JSON.parse(msgArr[1]);
  } catch (err) {
    console.log(`Error: data = ${msgArr[1]} is illformed.`)
    data = msgArr[1];
  }
  return {
    status: (msgArr[0] === 'ok'),
    data,
  }
}

module.exports = {
  parseMessage,
};
