import { GameInfo } from './gameManager';

export interface Mapped<T> {[key: string]: T}
export interface StrictMapped<T> extends Mapped<T | undefined> {}

export const NotificationType = {
  0: 'cycle_info',
  1: 'game_started',
  2: 'game_finished',
  3: 'client_dead',
  4: 'client_win',
}

export const parseMessage = (message: string) => {
  const msgArr = message.split('|', 2)
  let data: {} | undefined
  try {
    data = JSON.parse(msgArr[1])
  } catch (err) {
    console.log(`Error: data = ${msgArr[1]} is illformed.`)
    data = msgArr[1]
  }
  return {
    status: msgArr[0] === 'ok',
    data,
  }
}

export const parseNotif = (message: string) => {
  try {
    return JSON.parse(message) as {notification_type: string, data: GameInfo}
  } catch (err) {
    console.log(`Error: data = ${message} is illformed.`)
    return
  }
}
