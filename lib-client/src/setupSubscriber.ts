import * as zmq from 'zeromq'
import { parseNotif, StrictMapped } from './utils'
import { Config } from './config'
import { GameInfo } from './gameManager';

export interface SubscriberConfig extends Config {
  identity: string
}

export const setupSubscriber = ({PROTOCOL_PUB, ADDR_PUB, PORT_PUB, identity}: SubscriberConfig, events: StrictMapped<(info: GameInfo) => void>) => {
  console.log('Starting sub...')
  const subscriber = zmq.socket('sub')
  if (identity)
    subscriber.identity = identity

  subscriber.on('message', (...chunks) => {
    const data = parseNotif(chunks[0].toString())
    if (!data)
      return
    const event = events[data.notification_type]
    if (event)
      event(data.data)
  })

  subscriber.connect(`${PROTOCOL_PUB}://${ADDR_PUB}:${PORT_PUB}`)
  subscriber.subscribe('')

  process.on('SIGINT', function() {
    subscriber.unsubscribe('')
    subscriber.close()
  })
}
