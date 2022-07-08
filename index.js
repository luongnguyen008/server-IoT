const express = require('express')
const path = require('path')
const PORT = process.env.PORT || 5000
const { WebSocketServer, WebSocket } = require('ws');

const server = express()
  .use(express.static(path.join(__dirname, 'public')))
  .set('views', path.join(__dirname, 'views'))
  .set('view engine', 'ejs')
  .get('/', (req, res) => res.render('index'))
  .listen(PORT, () => console.log(`Listening on ${ PORT }`))


const wss = new WebSocketServer({ server });

wss.on('connection', (ws) => {
  console.log('Client connected');
  ws.on('message', function(message) {
    let data = message.toJSON().data
    let stringData = bin2String(data)
    console.log('Data', JSON.parse(stringData));
    wss.clients.forEach(function each(client) {
        if (client.readyState === WebSocket.OPEN) {
          client.send(stringData);
        }
    });
  });
  ws.on('close', () => console.log('Client disconnected'));
});
function bin2String(array) {
  return String.fromCharCode.apply(String, array);
}

