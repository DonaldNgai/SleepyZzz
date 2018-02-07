void sendData(float heartrate[], float movement[][4], float temperature[], int data_length){
  char header1[] = "POST /hello HTTP/1.1\r\nHost: 138.197.153.154:80\r\nContent-Type: application/json\r\nContent-Length: ";
  char header2[] = "\r\nUser-Agent: Arduino/1.0\r\nConnection: Close\r\n\r\n";
//  message = message + "{\"test\":\"test\"}\r\n\r\n\\0";
  String data = "{\"heartrate\":[";
  for(int i=0;i<data_length;i++){
    data = data + heartrate[i];
    if(!(i == data_length-1)){
      data = data + ",";
    }
  }
  data = data + "],\"movement\":[";
  for(int i=0;i<data_length;i++){
    data = data + "{";
    data = data + "\"x\":" + movement[i][0] + ",";
    data = data + "\"y\":" + movement[i][1] + ",";
    data = data + "\"z\":" + movement[i][2] + ",";
    data = data + "\"fall\":" + movement[i][3];
    data = data + "}";
    if(!(i == data_length-1)){
      data = data + ",";
    }
  }
  data = data + "],\"temperature\":[";
  for(int i=0;i<data_length;i++){
    data = data + temperature[i];
    if(!(i == data_length-1)){
      data = data + ",";
    }
  }
  data = data + "]}";

  char * dta = new char [data.length()];
  strcpy(dta, data.c_str());
  
  esp8266.write("AT+CIPSTART=\"TCP\",\"138.197.153.154\",80,1000\r\n");
  while(!esp8266.available());
  while(esp8266.available()){
    Serial.write(esp8266.read());
  }
  
  delay(100);
          
  esp8266.write("AT+CIPSENDEX=2048\r\n");
  while(!esp8266.available());
  while(esp8266.available()){
    Serial.write(esp8266.read());
  }
  
  delay(100);
  
  esp8266.write(header1);
  esp8266.print(data.length());
  esp8266.write(header2);
  esp8266.write(dta);
  esp8266.write("\r\n\r\n\\0");
  while(!esp8266.available());
  while(esp8266.available()){
    Serial.write(esp8266.read());
  }
}
