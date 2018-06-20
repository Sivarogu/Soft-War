const NotificationType = {
  0: 'cycle_info',
  1: 'game_started',
  2: 'game_finished',
  3: 'client_dead',
  4: 'client_win',
};

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

function parseNotif(message) {
  let data = null;
  try {
    data = JSON.parse(message);
  } catch (err) {
    console.log(`Error: data = ${message} is illformed.`);
  };
  return data;
}

module.exports = {
  parseMessage,
  parseNotif,
  NotificationType,
};
