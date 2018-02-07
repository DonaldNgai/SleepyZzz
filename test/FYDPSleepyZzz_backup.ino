#include <SparkFun_ADXL345.h>

#include <SoftwareSerial.h>
#define loopDurationInMillis  10000

SoftwareSerial esp8266(2,3); //RX - pin 2, TX - pin 3
int tmpPin = 0;
int rawTmpVoltage;
float celsiusTmp, tmpMilliVolts;
int val = 0;
int startTime = 0;
char c = 0;
int counter = 0;
char handshake[] = "GET /socket.io/?EIO=3&transport=websocket HTTP/1.1\r\nHost: 138.197.153.154\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nSec-WebSocket-Version: 13\r\nOrigin: http://localhost\r\nUser-Agent: Arduino/1.0\r\n\r\n\\0";
char example[] = "POST /hello HTTP/1.1\r\nHost: 138.197.153.154:80\r\nContent-Type: application/json\r\nContent-Length: 15\r\nUser-Agent: Arduino/1.0\r\nConnection: Close\r\n\r\n{\"test\":\"test\"}\r\n\r\n\\0";
ADXL345 adxl = ADXL345();             // USE FOR I2C COMMUNICATION

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Ready!");
  esp8266.begin(9600);
}

void sendPing(){
//  Serial.print(0x89,HEX);
//  Serial.print(0x82,HEX);
//  Serial.print(0xAABB,HEX);
//  Serial.println(0xFFFF,HEX);
//  Serial.print(0x89,BIN);
//  Serial.print(0x82,BIN);
//  Serial.print(0xAABB,BIN);
//  Serial.println(0xFFFF,BIN);
  
  esp8266.write("AT+CIPSENDEX=2048\r\n");
  while(!esp8266.available());
  while(esp8266.available()){
    Serial.write(esp8266.read());
  }
  esp8266.write(0x8900);
//  esp8266.write("\\0");
}

void sendJSON(String RID, String JSON) {
    esp8266.write("AT+CIPSENDEX=2048\r\n");
    while(!esp8266.available());
    delay(50);
    while(esp8266.available()){
      Serial.write(esp8266.read());
    }
    String message = "42[\"" + RID + "\"," + JSON + "]";
    int header[10];
    header[0] = 0x81;
    int msglength = message.length();
    randomSeed(analogRead(0));
    String mask = "";
    String masked = message;
    for (int i = 0; i < 4; i++)
    {
        char a = random(48, 57);
//        char a = random(0);
        mask += a;
    }

//    mask = 0x0000;

    char maskarray[mask.length()];
    
//    Serial.println(mask);
//    strcpy(maskarray, mask.c_str());
//    Serial.write(maskarray);
    
    for (int i = 0; i < msglength; i++)
        masked[i] = message[i] ^ mask[i % 4];
    char maskedarray[masked.length()];

    esp8266.write((char)header[0]);  //has to be sent for proper communication
                                    //Depending on the size of the message
    Serial.println("0");
    Serial.write((char)header[0]);
    delay(1000);
    if (msglength <= 125)
    {
//      Serial.println("if");
        header[1] = msglength + 128;
        esp8266.write((char)header[1]);  //size of the message + 128 because message has to be masked
        Serial.println("1");
        Serial.write((char)header[1]);
        delay(1000);
    }
    else if (msglength >= 126 && msglength <= 65535)
    {
//      Serial.println("else if");
        header[1] = 126 + 128;
        esp8266.write((char)header[1]);
        header[2] = (msglength >> 8) & 255;
        esp8266.write((char)header[2]);
        header[3] = (msglength)& 255;
        esp8266.write((char)header[3]);
//        Serial.println(header[1], HEX);
//    delay(1000);
//        Serial.println(header[2], HEX);
//    delay(1000);
//        Serial.println(header[3], HEX);
//    delay(1000);
    }
    else
    {
//        Serial.println("else");
//    delay(1000);
        header[1] = 127 + 128;
        esp8266.write((char)header[1]);
        header[2] = (msglength >> 56) & 255;
        esp8266.write((char)header[2]);
        header[3] = (msglength >> 48) & 255;
        esp8266.write((char)header[4]);
        header[4] = (msglength >> 40) & 255;
        esp8266.write((char)header[4]);
        header[5] = (msglength >> 32) & 255;
        esp8266.write((char)header[5]);
        header[6] = (msglength >> 24) & 255;
        esp8266.write((char)header[6]);
        header[7] = (msglength >> 16) & 255;
        esp8266.write((char)header[7]);
        header[8] = (msglength >> 8) & 255;
        esp8266.write((char)header[8]);
        header[9] = (msglength)& 255;
        esp8266.write((char)header[9]);
        
//        Serial.println(header[1], HEX);
//    delay(1000);
//        Serial.println(header[2], HEX);
//    delay(1000);
//        Serial.println(header[3], HEX);
//    delay(1000);
//        Serial.println(header[4], HEX);
//    delay(1000);
//        Serial.println(header[5], HEX);
//    delay(1000);
//        Serial.println(header[6], HEX);
//    delay(1000);
//        Serial.println(header[7], HEX);
//    delay(1000);
//        Serial.println(header[8], HEX);
//    delay(1000);
//        Serial.println(header[9], HEX);
//    delay(1000);
    }
    strcpy(maskarray, mask.c_str());
    esp8266.write(maskarray);
    Serial.println("mask");
    Serial.write(maskarray);
    delay(1000);
    strcpy(maskedarray, masked.c_str());
    esp8266.write(maskedarray);
    Serial.println("masked");
    Serial.write(maskedarray);
    esp8266.write("\\0");
}

void loop() {
    // listen for communication from the ESP8266 and then write it to the serial monitor
//  if (esp8266.available()) {
//    Serial.write(esp8266.read());
//  }
//  if (Serial.available()) {
//    esp8266.write(Serial.read());
//  }
    if ( esp8266.available() )   {  
      c = esp8266.read();
      Serial.write(c);
    }
//
//    if ( Serial.available() )   {  
//      c = Serial.read();
//      esp8266.write(c);
//    }
    
    // listen for user input and send it to the ESP8266
    if ( Serial.available() )    {  
      c = Serial.read();

      if(c == 'w'){
        
        delay(250);
        Serial.write("\r\nOpening DB Connection\r\n");
        delay(250);
                
        esp8266.write("AT+CIPSTART=\"TCP\",\"138.197.153.154\",80,5000\r\n");
        while(!esp8266.available());
        while(esp8266.available()){
          Serial.write(esp8266.read());
        }
        
        delay(250);
        Serial.write("Sending data\r\n");
        delay(250);
                
        esp8266.write("AT+CIPSENDEX=2048\r\n");
        while(!esp8266.available());
        while(esp8266.available()){
          Serial.write(esp8266.read());
        }
        
        delay(250);
        Serial.write("Writing data\r\n");
        delay(250);
        
//        esp8266.write(handshake);
        esp8266.write(example);
        while(!esp8266.available());
        while(esp8266.available()){
          Serial.write(esp8266.read());
        }
//        delay(250);
//        while(!esp8266.available());
//        while(esp8266.available()){
//          Serial.write(esp8266.read());
//        }
//
//      }else if(c == 'q'){
//
//        Serial.write("\r\nSending Data\r\n");
//        delay(500);
//        
//        sendJSON("heartrate", "{\"bpm\":125.38321708604502,\"device\":\"-KpeRPjghebc5YjXyFSf\"}");
//        
      }else if(c == 'q'){
        sendPing();
        delay(50);
      } else {
        esp8266.write(c);
      }
    }
    
}
