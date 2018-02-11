
void connectServer(){
  esp8266.print(F("AT+CIPSTART=\"TCP\",\"138.197.153.154\",80,1000\r\n"));
  while(!esp8266.available());
  while(esp8266.available()){
    Serial.write(esp8266.read()); // These need to be here as there is some kind of timing thing that will cause Wifi to fail if removed
  }

  delay(100);
}

void openDataChannel(){
  //IMPORTANT!!!! YOU MUST SEND "\r\n\r\n\\0" TO CLOSE THIS DATA CHANNEL!!!
  esp8266.print(F("AT+CIPSENDEX=2048\r\n"));
  while(!esp8266.available());
  while(esp8266.available()){
    Serial.write(esp8266.read()); // These need to be here as there is some kind of timing thing that will cause Wifi to fail if removed
  }
  delay(100);
}

void tokenRequest(){
//GET /api/v1/token?device=-KpeRPjghebc5YjXyFSf HTTP/1.1
//Host: 138.197.153.154
//Content-Type: application/json
//Authorization: Basic c2xlZXB5enp6OkRFNEYxQzE3LTMwOEQtNEY0OS04MjU2LTRERTlFN0M5QjhDQg==
    connectServer();
    openDataChannel();

    esp8266.print(F("GET /api/v1/token?device=-KpeRPjghebc5YjXyFSf HTTP/1.1\r\n"));
    esp8266.print(F("Host: 138.197.153.154\r\n"));
    esp8266.print(F("Content-Type: application/json\r\n"));
    esp8266.print(F("Authorization: Basic c2xlZXB5enp6OkRFNEYxQzE3LTMwOEQtNEY0OS04MjU2LTRERTlFN0M5QjhDQg==\r\n\r\n\\0"));

    delay(200);
    
    char c = "\0";
    int output_index = 0;
     
    while (true){
        delay(100);
        while(esp8266.available()){
            c = esp8266.read();
//            token.append(c);
//            token.concat(c);
            token[output_index] = c;
            output_index++;
            token[output_index] = "\0";
            
            if (c == '}') {
              return;
            }
          }
    
        while(!esp8266.available()){} 
    }
}

void sendData(float heartrate[], float movement[3][DATA_STORAGE_DEPTH], bool fall[], float temperature[]){

  connectServer();
  openDataChannel();

  esp8266.print(F("POST /api/v1/data?token="));
  esp8266.write(token);
  esp8266.print(F(" HTTP/1.1\r\nHost: 138.197.153.154\r\nContent-Type: application/json\r\nContent-Length: "));
  esp8266.print(233);
  esp8266.print(F("\r\nUser-Agent: Arduino/1.0\r\nAuthorization: Basic c2xlZXB5enp6OkRFNEYxQzE3LTMwOEQtNEY0OS04MjU2LTRERTlFN0M5QjhDQg==\r\nConnection: Close\r\n\r\n"));

  strcpy(data,"{\"heartrate\":[");
  for(int i=0;i<DATA_STORAGE_DEPTH;i++){
    dtostrf(heartrate[i], FLOAT_STRING_LENGTH, FLOAT_STRING_DECIMALS, val);
    strcat(data, val);
    if(!(i == DATA_STORAGE_DEPTH-1)){
      strcat(data, ",");
    }
  }
  
  strcat(data,"],\"movement\":[");
  for(int i=0;i<DATA_STORAGE_DEPTH;i++){
    strcat(data,"{");
    strcat(data,"\"x\":");
    dtostrf(movement[ACCEL_X][i], FLOAT_STRING_LENGTH, FLOAT_STRING_DECIMALS, val);
    strcat(data, val);
    strcat(data,",\"y\":");
    dtostrf(movement[ACCEL_Y][i], FLOAT_STRING_LENGTH, FLOAT_STRING_DECIMALS, val);
    strcat(data, val);
    strcat(data,",\"z\":");
    dtostrf(movement[ACCEL_Z][i], FLOAT_STRING_LENGTH, FLOAT_STRING_DECIMALS, val);
    strcat(data, val);
    strcat(data,",\"fall\":");
    strcat(data, (fall[i] ? "1" : "0"));
    strcat(data,"}");
    if(!(i == DATA_STORAGE_DEPTH-1)){
      strcat(data,",");
    }
  }
  
  strcat(data,"],\"temperature\":[");
  for(int i=0;i<DATA_STORAGE_DEPTH;i++){
    dtostrf(temperature[i], FLOAT_STRING_LENGTH, FLOAT_STRING_DECIMALS, val);
    strcat(data, val);
    if(!(i == DATA_STORAGE_DEPTH-1)){
      strcat(data,",");
    }
  }
  strcat(data,"]}");
    
  esp8266.write(data);
  esp8266.print(F("\r\n\r\n\\0"));

}
