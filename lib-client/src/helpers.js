function getRandomID(max = 10) {
  const random = Math.floor(Math.random() * max) + 1;
  return random.length > 10 ? random : `0${random}`;
}

module.exports = {
  getRandomID,
};
