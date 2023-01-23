import mqtt from 'mqtt'
let client = mqtt.connect("mqtt://192.168.0.131:1883");

import http from 'http';
import fs from 'fs';

const hostname = '127.0.0.1';
const port = 3000;

let topic="esp1/distance"
let message =  "on"
let water = "esp1/volume"
let watermessage;

const server = http.createServer((req, res) => {
  res.statusCode = 200;
  res.setHeader('Content-Type', 'text/html');
  fs.readFile("./index.html",null, (error, data)=>{
    if (error) {
        res.writeHead(404)
    }else{
        res.write(data)
    }
    res.end()
  })
});
server.listen(port, hostname, () => { 

    // Message Publisher
    let publisher = (topic, message)=>{
        if (client.connected == true) {
            client.publish(topic, message);
        }
    }
    client.on('connect', ()=>{
        if (client.connected == true) {
            publisher(topic, message);
        }
    })
    // Subscribe to Messages
    client.subscribe(water);
    client.on('message', (topic, watermessage)=>{
        console.log("message is: " + watermessage);
        console.log("topic is: " + topic);
    });




    client.on('error', function(error) {
        console.log("Unable to connect: " + error);
        process.exit(1);
    });
});