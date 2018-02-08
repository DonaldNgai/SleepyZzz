void sendData(float heartrate[], float movement[3][DATA_STORAGE_DEPTH], bool fall[], float temperature[]){
//  char header1[] = "POST /hello HTTP/1.1\r\nHost: 138.197.153.154:80\r\nContent-Type: application/json\r\nContent-Length: ";
//  message = message + "{\"test\":\"test\"}\r\n\r\n\\0";

  strcpy(data,"{\"heartrate\":[");
  for(int i=0;i<DATA_STORAGE_DEPTH;i++){
    dtostrf(heartrate[i], FLOAT_STRING_LENGTH, FLOAT_STRING_DECIMALS, val);
    strcat(data, val);
    if(!(i == DATA_STORAGE_DEPTH-1)){
      strcat(data, ",");
    }
  }
  
//  Serial.println(data);
  
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
//  Serial.println(data);
  
  strcat(data,"],\"temperature\":[");
  for(int i=0;i<DATA_STORAGE_DEPTH;i++){
    dtostrf(temperature[i], FLOAT_STRING_LENGTH, FLOAT_STRING_DECIMALS, val);
    strcat(data, val);
    if(!(i == DATA_STORAGE_DEPTH-1)){
      strcat(data,",");
    }
  }
  strcat(data,"]}");

  Serial.println(data);
  
//  esp8266.write("AT+CIPSTART=\"TCP\",\"138.197.153.154\",80,1000\r\n");
//  while(!esp8266.available());
//  while(esp8266.available()){
//    Serial.write(esp8266.read());
//  }
//  
//  delay(100);
//          
//  esp8266.write("AT+CIPSENDEX=2048\r\n");
//  while(!esp8266.available());
//  while(esp8266.available()){
//    Serial.write(esp8266.read());
//  }
  
//  delay(100);
  
//  esp8266.write(header1);
//  esp8266.print(data.length());
//  esp8266.write(header2);
//  esp8266.write(dta);
//  esp8266.write("\r\n\r\n\\0");
//  Serial.write(header1);
//  Serial.print(data.length());
//  Serial.write(header2);
//  Serial.write(dta);
//  Serial.write("\r\n\r\n\\0");
//  while(!esp8266.available());
//  while(esp8266.available()){
//    Serial.write(esp8266.read());
//  }

  Serial.flush();
//  delete data;
//  data = NULL;
//  delete val;
//  val = NULL;

}
