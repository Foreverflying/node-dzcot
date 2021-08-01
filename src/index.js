const dzcot = require('../build/Debug/node_dzcot')
// import { dzRunHost } from '../build/Release/node_dzcot.node'

let a = 2

const hello1 = function () {
    console.log('world 1', a)
    a += 1
}

const hello2 = () => {
    console.log('world 2', a)
    a += 2
}

const result1 = dzcot.hello()
console.log('result1:', result1)
const result2 = dzcot.dzRunHost(8192, 32768, 65536, 2, 2, 2, hello1)
console.log('result2:', result2)
const result3 = dzcot.dzRunHost(8192, 32768, 65536, 2, 2, 2, hello2)
console.log('result3:', result3)