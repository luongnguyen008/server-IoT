const express = require('express')
const path = require('path')
const PORT = process.env.PORT || 5000
const { WebSocketServer, WebSocket } = require('ws');

const server = express()
  .use(express.static(path.join(__dirname, 'public')))
  .set('views', path.join(__dirname, 'views'))
  .set('view engine', 'ejs')
  .get('/', (req, res) => res.render('index'))
// .get('/', (req, res) => {
//   res.send('Hello World!')
// })
  .listen(PORT, () => console.log(`Listening on ${ PORT }`))


const wss = new WebSocketServer({ server });
wss.on('connection', (ws) => {
  console.log('Client connected');
  ws.on('message', function(message) {
    console.log('received:', message, typeof message);
    let data = message.toJSON().data
    console.log('data', data, typeof data );
    let stringData = bin2String(data)
    console.log('stringData', stringData, typeof stringData );
    let result = JSON.parse(stringData)
    console.log('result', result, typeof result );
    wss.clients.forEach(function each(client) {
      if (client.readyState === WebSocket.OPEN) {
        client.send("abc");
      }
    });
    
  });
  ws.on('close', () => console.log('Client disconnected'));
});
function bin2String(array) {
  return String.fromCharCode.apply(String, array);
}

