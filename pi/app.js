require('dotenv').config()
const express = require('express');
const app = express();
const http = require('http').createServer(app);
const io = require('socket.io')(http);
const mongoose = require('mongoose');
const Sensors = require('./models/sensors');

const SerialPort = require("serialport").SerialPort;
const xbee_api = require('xbee-api');

const path = require('path');

const fs = require('fs');

app.use(express.static(path.join(__dirname, 'public')));

app.get('/', (req, res) => res.sendFile('index.html'));

io.on('connection', (socket) => {
    socket.emit('updateClient', "Hello");
});

const PORT = process.env.PORT || 3002;

http.listen(PORT, () => {
  console.log(`Listening on port ${PORT}`);
});

//type line below to find port xbee is attached to
//ls /dev/tty*
const xbeeSerial = new SerialPort({
  path:"/dev/ttyUSB1",
  baudRate: 9600
});

var xbeeAPI = new xbee_api.XBeeAPI({ api_mode: 1 });
xbeeSerial.pipe(xbeeAPI.parser);

xbeeSerial.on('open', () => console.log("Xbee Port in now Open"));

mongoose.connect(process.env.URI, {useNewUrlParser: true});

const con = mongoose.connection;

con.on('open', () => console.log('mongo connected'));

let increment = 0;

xbeeAPI.parser.on("data", frame => {

    if (typeof frame != 'undefined'){
        //146 - IO Sample - radio sent capture frame.digitalSamples
        //144 - Zigbee packet - we sent
        if (frame.type == 144){
          let randomNumber = Math.floor(Math.random()*schools.length);
          let school = schools[randomNumber];
            //light range 0: darkest to 1024: brightest
            let light = byteArrayToInt([frame.data[0],frame.data[1]]);
            let temp = byteArrayToFloat([frame.data[2], frame.data[3], frame.data[4], frame.data[5]]);
            let timestamp = new Date().toISOString();

            let sensordata = {
              temp,
              light
            }
            //send to mongo
            send(sensordata);
            //inform front end of new data
            io.sockets.emit('statedata', {school, light,temp,timestamp})
        }
    }
});

let schools = [
  "SchoolA",
  "SchoolB",
  "SchoolC"
]

const send = async (data) => {
  const sensorData =  Sensors(
    data
  );
  console.log(data);
  let res = await sensorData.save();
}


//https://stackoverflow.com/questions/42699162/javascript-convert-array-of-4-bytes-into-a-float-value-from-modbustcp-read
let byteArrayToInt = (byteArray) => {
  return Buffer.from([byteArray[0], byteArray[1]]).readInt16BE(0);
}

let byteArrayToFloat = (byteArray) => {
  return Buffer.from([ byteArray[0], byteArray[1], byteArray[2], byteArray[3] ]).readFloatLE(0).toFixed(2);
};

  /*
  {
    "username": "SCHOOLD",
    "data": {
      "school": "SchoolD",
      "light": 324,
      "temp": 66.2,
      "timestamp": "2023-03-17T20:04:05.845Z"
    },
    "timestamp": "2023-03-17T20:04:05.845Z"
  }
  */