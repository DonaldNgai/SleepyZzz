char token[211] = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJkZXZpY2UiOiItS3BlUlBqZ2hlYmM1WWpYeUZTZiIsImJhYnkiOiItS3BlZGdVZkRPU1AwZFRnb0ZQMCIsImlhdCI6MTUxODExODUwNywiZXhwIjoxNTE4MjA0OTA3fQ.wts8u_hzJ9WbGbx4FJ2ToGCyVaA7GNCmmYDxaLAPRFY";

void sendData(float heartrate[], float movement[3][DATA_STORAGE_DEPTH], bool fall[], float temperature[]){
//  char header1[] = "POST /hello HTTP/1.1\r\nHost: 138.197.153.154:80\r\nContent-Type: application/json\r\nContent-Length: ";
//  message = message + "{\"test\":\"test\"}\r\n\r\n\\0";

  esp8266.print(F("AT+CIPSTART=\"TCP\",\"138.197.153.154\",80,1000\r\n"));
  while(!esp8266.available());
  while(esp8266.available()){
    Serial.write(esp8266.read());
  }

  delay(100);

  esp8266.print(F("AT+CIPSENDEX=2048\r\n"));
  while(!esp8266.available());
  while(esp8266.available()){
    Serial.write(esp8266.read());
  }
  delay(100);

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
//  delay(1000);
//  while(!esp8266.available());
//  while(esp8266.available()){
//    Serial.write(esp8266.read());
//  }

//  Serial.flush();


}
