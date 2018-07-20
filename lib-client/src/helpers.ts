export const getRandomID = (max: number = 10) => {
  const random = Math.floor(Math.random() * max) + 1
  return random.toString().length > 10 ? random : `0${random}`
}
