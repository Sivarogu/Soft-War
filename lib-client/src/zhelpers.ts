// Return the buffer's length as a three-character,
// zero-padded string (e.g. printf's `%03d`)
const bufferLength = (buffer: Buffer) => {
  let lenStr = '' + buffer.length
  while (lenStr.length < 3)
    lenStr = '0' + lenStr
  return lenStr
}

// Return the buffer's contents as printable text if every
// character is printable, or as hexadecimal otherwise
const formatBuffer = (buffer: Buffer) => {
  for (let i = 0; i < buffer.length; i++)
    if (buffer[i] < 32 || buffer[i] > 127)
      return buffer.toString('hex')
  return buffer.toString('utf8')
}

export const dumpFrames = (...frames: Array<Buffer>) => {
  console.log('----------------------------------------')
  frames.forEach(frame => console.log('[%s] %s', bufferLength(frame), formatBuffer(frame)))
}
