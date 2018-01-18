#include <SparkFun_ADXL345.h>

#include <SoftwareSerial.h>
#define loopDurationInMillis  10000

SoftwareSerial esp8266(2,3); //RX - pin 2, TX - pin 3
int tmpPin = 0;
int rawTmpVoltage;
float celsiusTmp, tmpMilliVolts;
int val = 0;
int startTime = 0;
ADXL345 adxl = ADXL345();             // USE FOR I2C COMMUNICATION

void setup() {
  Serial.begin(115200);
  esp8266.begin(115200);
}

void loop() {
  
    if (millis() - startTime > loopDurationInMillis){
      startTime = millis();

      rawTmpVoltage= analogRead(tmpPin); 
      tmpMilliVolts = rawTmpVoltage * 4.9; 
      celsiusTmp= (tmpMilliVolts/20) - 50; 
      
      Serial.print(celsiusTmp); 
//      Serial.println(" Celsius"); 
    }
    
    // listen for communication from the ESP8266 and then write it to the serial monitor
    if ( esp8266.available() )   {  Serial.write( esp8266.read() );  }
 
    // listen for user input and send it to the ESP8266
    if ( Serial.available() )    {  esp8266.write( Serial.read() );  }
}
