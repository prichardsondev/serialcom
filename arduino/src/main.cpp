#include <Arduino.h>
#include <XBee.h>
#include <SoftwareSerial.h>
#include "DHT.h"

XBee xbee = XBee();
uint8_t payload[] = {0, 0, 0, 0, 0, 0, 0, 0};

uint8_t ssRX = 2;
uint8_t ssTX = 3;

SoftwareSerial xbeeSerial(ssRX, ssTX);

XBeeAddress64 addr64 = XBeeAddress64(0x0, 0x0);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

int lightSensorReading = 0;
#define LIGHT_SENSOR_PIN A0

float tempSensorReading = 0;
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

unsigned long interval = 1000;
unsigned long previousMillis = 0;

#define DEBUG true

void setup() {
  
  if (DEBUG)
  {
    Serial.begin(9600);
    xbeeSerial.begin(9600);
    xbee.begin(xbeeSerial);
    Serial.println("debug mode");
  }
  else
  {
    Serial.begin(9600);
    xbee.begin(Serial);
  }

  dht.begin();
}

void loop() {   

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > interval)
  {
    previousMillis = currentMillis;

    lightSensorReading = analogRead(LIGHT_SENSOR_PIN);
    tempSensorReading = dht.readTemperature(true);

    //https://stackoverflow.com/questions/64234007/how-do-i-add-float-values-to-a-byte-array-in-c-arduino
    byte dataArray[4] = {
        ((uint8_t*)&tempSensorReading)[0],
        ((uint8_t*)&tempSensorReading)[1],
        ((uint8_t*)&tempSensorReading)[2],
        ((uint8_t*)&tempSensorReading)[3]
    };

    payload[0] = lightSensorReading >> 8 & 0xff;
    payload[1] = lightSensorReading & 0xff;
    payload[2] = dataArray[0];
    payload[3] = dataArray[1];
    payload[4] = dataArray[2];
    payload[5] = dataArray[3];
  

    xbee.send(zbTx);


    if(DEBUG){
      Serial.print("Light Level: ");
      Serial.print(lightSensorReading);
      Serial.print(" ");
      Serial.print("Temp: ");
      Serial.println(tempSensorReading);
    }
  }

}


  // // after sending a tx request, we expect a status response
  // // wait up to half second for the status response
  // if (xbee.readPacket(500)) {
  //   // got a response!

  //   // should be a znet tx status            	
  //   if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
  //     xbee.getResponse().getZBTxStatusResponse(txStatus);

  //     // get the delivery status, the fifth byte
  //     if (txStatus.getDeliveryStatus() == SUCCESS) {

  //     } else {
  //       Serial.println("fail");
  //     }
  //   }
  // } else if (xbee.getResponse().isError()) {
  //   //nss.print("Error reading packet.  Error code: ");  
  //   //nss.println(xbee.getResponse().getErrorCode());
  // } else {
  //   // local XBee did not provide a timely TX Status Response -- should not happen
   
  // }

  // delay(1000);
