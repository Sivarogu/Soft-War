import * as zmq from 'zeromq'

export const registerMonitor = (requester: zmq.Socket) => {
  requester.on('connect', (fd, ep) => console.log('MONITOR -- connect, endpoint:', ep))
  requester.on('connect_delay', (fd, ep) => console.log('MONITOR -- connect_delay, endpoint:', ep))
  requester.on('connect_retry', (fd, ep) => console.log('MONITOR -- connect_retry, endpoint:', ep))
  requester.on('listen', (fd, ep) => console.log('MONITOR -- listen, endpoint:', ep))
  requester.on('bind_error', (fd, ep) => console.log('MONITOR -- bind_error, endpoint:', ep))
  requester.on('accept', (fd, ep) => console.log('MONITOR -- accept, endpoint:', ep))
  requester.on('accept_error', (fd, ep) => console.log('MONITOR -- accept_error, endpoint:', ep))
  requester.on('close', (fd, ep) => console.log('MONITOR -- close, endpoint:', ep))
  requester.on('close_error', (fd, ep) => console.log('MONITOR -- close_error, endpoint:', ep))
  requester.on('disconnect', (fd, ep) => console.log('MONITOR -- disconnect, endpoint:', ep))

  // Handle monitor error
  requester.on('monitor_error', (err) => {
    console.log('Error in monitoring: %s, will restart monitoring in 5 seconds', err)
    setTimeout(() => requester.monitor(500, 0), 5000)
  })
}
