export interface Config {
  PROTOCOL_ROUTER: 'tcp'
  ADDR_ROUTER: string
  PORT_ROUTER: number
  PROTOCOL_PUB: 'tcp'
  ADDR_PUB: string
  PORT_PUB: number
  IDENTITY_BASE: string
  RETRY: number
}

export const config: Config = {
  PROTOCOL_ROUTER: 'tcp',
  ADDR_ROUTER: '192.168.171.132',
  PORT_ROUTER: 4242,
  PROTOCOL_PUB: 'tcp',
  ADDR_PUB: '192.168.171.132',
  PORT_PUB: 12345,
  IDENTITY_BASE: '#0x',
  RETRY: 5,
}
