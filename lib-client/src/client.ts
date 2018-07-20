import {com} from './com'
import {config} from './config'

const { Manager, connected } = com(config)
// extend Manager to have cycle and game start event or use as is
// example
const manager = new Manager()

connected.then((signals) => {
  // do stuff here with Signals
  // example
  signals.watch()
    .then((rep) => {
      console.log('watch !', rep)
    })
    .catch((err) => {
      console.log('err')
    })

  signals.selfstats()
    .then((rep) => {
      console.log('selfstats !', rep)
    })
    .catch((err) => {
      console.log('err')
    })
})
