import * as zmq from 'zeromq'
import { getRandomID } from './helpers'
import { parseMessage } from './utils'
import { Config } from './config'

export const setup = ({ PROTOCOL_ROUTER, ADDR_ROUTER, PORT_ROUTER, RETRY, IDENTITY_BASE }: Config) => {
  const waitingForRep: Array<(rep: string) => void> = []

  console.log('Starting Client...')
  const requester = zmq.socket('req')
  requester.monitor(500, 0)

  let identity = `${IDENTITY_BASE}${getRandomID(10)}`
  requester.identity = identity

  const send = <TData>(command: string, data: TData) => {
    const msg = `${command}|${JSON.stringify(data)}`
    requester.send(msg)

    return new Promise((resolve, reject) => {
      waitingForRep.push((rep) => {
        const { status, data } = parseMessage(rep)
        return (status ? resolve : reject)(data)
      })
    })
  }

  const listenForReplies = () =>
    requester.on('message', (reply) => {
      const callback = waitingForRep.shift()
      if (callback)
        callback(reply.toString())
    })

  const identify = async () => {
    console.log('Identifying client to server...')
    let attemptsCount = 1
    let identified = false
    while (!identified && attemptsCount < RETRY) {
      requester.identity = identity
      try {
        await send('identify', identity)
        identified = true
      }
      catch (err) {
        identity = `${IDENTITY_BASE}${getRandomID(10)}`
      }
    }
    if (!identified)
      throw new Error('Couldn\'t identify to server.')
  }

  listenForReplies()

  process.on('SIGINT', () => requester.close())

  requester.connect(`${PROTOCOL_ROUTER}://${ADDR_ROUTER}:${PORT_ROUTER}`)

  const done = new Promise((resolve) => {
    requester.on('connect', async (fd, ep) => {
      console.log('MONITOR -- connect, endpoint:', ep)
      await identify()
      console.log(`Successfuly identified to server as ${identity}`)
      resolve()
    })
  })

  return { requester, identity, send, done }
}
