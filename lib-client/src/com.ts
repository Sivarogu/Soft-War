import {setup}  from './setupReq'
import {getGameManager} from './gameManager'
import {Config} from './config'
import { SubscriberConfig } from './setupSubscriber'
import { getSignals } from './signals';

export const com = (config: Config) => {
  const {identity, send, done} = setup(config)
  const signals = getSignals(send)
  const Manager = getGameManager({identity, ...config})
  return {
    Manager,
    connected: done.then(() => {
      return signals
    }),
  }
}
