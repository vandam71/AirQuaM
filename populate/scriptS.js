const axios = require('axios')

axios.post('https://localhost:3000/data/station', {
        name: 'test',
        sampleRate: 10,
        activeSensors: 20

}).then((res) => {
    console.log(`statusCode: ${res.statusCode}`)
    console.log(res)
}).catch((error) => {
    console.error(error)
})