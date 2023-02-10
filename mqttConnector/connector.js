import express from 'express'
import { createServer } from 'http';
import {Server} from 'socket.io'
import mqtt from 'mqtt'
import mysql from 'mysql2'
import path from 'path'


const client = mqtt.connect('mqtt://192.168.0.203');
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
  client.subscribe(['esp1/distance','esp1/volume','esp1/duration'], function(err) {
    if (!err) {
      console.log('Successfully subscribed to topicS: volume  distance and delay');
    }
  });
});

client.on('message', function(topic, message) {
  // Parse the message into a JavaScript object.
  // .trim truncates the space in the data.
  //const data= message.toString().trim();
  // Store the message in the database:
  //console.log(data)
  storeMessageInDb(topic,message);

 
});

let n =1;

function storeMessageInDb(topic,message) {
  const connection = mysql.createConnection({
    host: '192.168.0.203',  
    user: 'root',
    password: '2340917zC',
    database: 'somatank'
  });
 if(topic =='esp1/distance'){
  connection.query(
    `INSERT INTO pumpstats (sep_distance) VALUES (?)`,
    [message.toString().trim()],
    function(error, results, fields) {
      if (error) throw error;
      console.log('volume has been inserted into the database.');
      connection.end();
    }
  );
 }
  else if(topic =='esp1/duration'){
    connection.query(`UPDATE pumpstats set delay = (?) WHERE delay IS NULL`,
    [message.toString().trim(),n],
    (err,res)=>{
      console.log("Delay has been inserted successfully");
      n+=1;
      connection.end();
    }
    );
  }
  else if(topic =='esp1/volume'){
    connection.query(`UPDATE pumpstats set volume = (?) WHERE volume IS NULL`,
    [message.toString().trim(),n],
    (err,res)=>{
      console.log("Delay has been inserted successfully");
      n+=1;
      connection.end();
    }
    );

  }
  connection.query(`SELECT volume FROM pumpstats ORDER BY id DESC LIMIT 1`,
    (err,result,fields)=>{
            io.on('connection',(socket)=>{
            console.log('A new user has connected');      
            console.log(result);
            socket.emit('data', result)
        })
  })
}

