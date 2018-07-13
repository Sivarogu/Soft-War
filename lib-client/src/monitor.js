module.exports = function registerMonitor(requester) {
  requester.on('connect', function(fd, ep) {
    console.log('MONITOR -- connect, endpoint:', ep);
  });
  requester.on('connect_delay', function(fd, ep) {console.log('MONITOR -- connect_delay, endpoint:', ep);});
  requester.on('connect_retry', function(fd, ep) {console.log('MONITOR -- connect_retry, endpoint:', ep);});
  requester.on('listen', function(fd, ep) {console.log('MONITOR -- listen, endpoint:', ep);});
  requester.on('bind_error', function(fd, ep) {console.log('MONITOR -- bind_error, endpoint:', ep);});
  requester.on('accept', function(fd, ep) {console.log('MONITOR -- accept, endpoint:', ep);});
  requester.on('accept_error', function(fd, ep) {console.log('MONITOR -- accept_error, endpoint:', ep);});
  requester.on('close', function(fd, ep) {console.log('MONITOR -- close, endpoint:', ep);});
  requester.on('close_error', function(fd, ep) {console.log('MONITOR -- close_error, endpoint:', ep);});
  requester.on('disconnect', function(fd, ep) {console.log('MONITOR -- disconnect, endpoint:', ep);});

  // Handle monitor error
  requester.on('monitor_error', function(err) {
    console.log('Error in monitoring: %s, will restart monitoring in 5 seconds', err);
    setTimeout(function() { requester.monitor(500, 0); }, 5000);
  });
}
