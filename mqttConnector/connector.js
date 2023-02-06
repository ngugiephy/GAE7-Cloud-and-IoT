import express from 'express'
import { createServer } from 'http';
import {Server} from 'socket.io'
import mqtt from 'mqtt'
import mysql from 'mysql2'
import path from 'path'


const client = mqtt.connect('mqtt://192.168.0.89');
const __dirname = path.resolve()
const publicPath = path.join(__dirname, '/public')
const app = express()
const httpServer = createServer(app)
const io = new Server(httpServer)
app.use(express.static(publicPath))

httpServer.listen(3000,()=>{
    console.log(`Server listening on port:3000`);
})



client.on('connect', function() {
  client.subscribe('volume', function(err) {
    if (!err) {
      console.log('Successfully subscribed to topic: volume');
    }
  });
});

client.on('message', function(topic, message) {
  // Parse the message into a JavaScript object:
  const volume = message.toString().trim();

  // Store the message in the database:
  console.log(volume);

  storeMessageInDb(volume);
});

function storeMessageInDb(volume) {
  const connection = mysql.createConnection({
    host: '192.168.0.89',
    user: 'root',
    password: '2340917zC',
    database: 'somatank'
  });

  connection.query(
    `INSERT INTO pumpstats (volume,epoch) VALUES (?, CURRENT_TIMESTAMP)`,
    [volume],
    function(error, results, fields) {
      if (error) throw error;
      console.log('Data has been inserted into the database.');
      connection.end();
    }
  );
  connection.query(`SELECT * FROM pumpstats WHERE volume IS NOT NULL`,
    (err,result,fields)=>{
        io.on('connection',(socket)=>{
            console.log('A new user has connected');
            console.log(result);
            socket.emit('data', result)
        })
  })
}



