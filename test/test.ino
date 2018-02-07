#include <SoftwareSerial.h>

//SoftwareSerial esp8266(2,3); //RX - pin 2, TX - pin 3
SoftwareSerial esp8266(2,3); //RX - pin 2, TX - pin 3

void setup() {
  Serial.begin(115200);
  esp8266.begin(115200);
}

void loop() {
    // listen for communication from the ESP8266 and then write it to the serial monitor
    if ( esp8266.available() )   {  Serial.write( esp8266.read() );  }
 
    // listen for user input and send it to the ESP8266
    if ( Serial.available() )    {  esp8266.write( Serial.read() );  }

//  esp8266.write("AT\r\n");
//  while(!esp8266.available());
//  while(esp8266.available()){
//    esp8266.read();
//  }
//  delay(1000);
}

