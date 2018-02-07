#include <SparkFun_ADXL345.h>

#include <SoftwareSerial.h>
#define loopDurationInMillis  10000

SoftwareSerial esp8266(2,3); //RX - pin 2, TX - pin 3
char c = 0;
int counter = 0;
char example[] = "POST /hello HTTP/1.1\r\nHost: 138.197.153.154:80\r\nContent-Type: application/json\r\nContent-Length: 15\r\nUser-Agent: Arduino/1.0\r\nConnection: Close\r\n\r\n{\"test\":\"test\"}\r\n\r\n\\0";
ADXL345 adxl = ADXL345();             // USE FOR I2C COMMUNICATION

void setup() {
  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }
//  Serial.println("Ready!");
  esp8266.begin(9600);
}


void loop() {
//    if ( esp8266.available() )   {  
//      c = esp8266.read();
//      Serial.write(c);
//    }
//
//    if ( Serial.available() )   {  
//      c = Serial.read();
//      esp8266.write(c);
//    }
    
    // listen for user input and send it to the ESP8266
//    if ( Serial.available() )    {  
//      c = Serial.read();
//
//      if(c == 'q'){
        float hr[3] = {1,2,3};
        float mv[3][4] = {{4,4,4,4},{5,5,5,5},{6,6,6,6}};
        float tmp[3] = {7,8,9};
        int data_length = sizeof(hr)/sizeof(hr[0]);
        sendData(hr,mv,tmp, data_length);
//      } else {
//        esp8266.write(c);
//      }
//    }

    Serial.println("\r\n\r\nDone\r\n\r\n");
    while(true){
      if ( esp8266.available() )   {  
        c = esp8266.read();
        Serial.write(c);
      }
  
      if ( Serial.available() )   {  
        c = Serial.read();
        esp8266.write(c);
      }
    }
}
