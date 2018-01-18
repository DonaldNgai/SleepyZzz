//#include <ESP8266WiFi.h>
//#include <ESP8266WiFiMulti.h>
//#include <ArduinoOTA.h>
//#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
//#include <FS.h>
//#include <WebSocketsServer.h>
//
//ESP8266WiFiMulti wifiMulti;       // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
//
//ESP8266WebServer server = ESP8266WebServer(80);       // create a web server on port 80
//WebSocketsServer webSocket = WebSocketsServer(81);    // create a websocket server on port 81
//
//File fsUploadFile;                                    // a File variable to temporarily store the received file
//
//const char *ssid = "ESP8266 Access Point"; // The name of the Wi-Fi network that will be created
//const char *password = "thereisnospoon";   // The password required to connect to it, leave blank for an open network
//
//const char *OTAName = "ESP8266";           // A name and a password for the OTA service
//const char *OTAPassword = "esp8266";
//
//#define LED_RED     15            // specify the pins with an RGB LED connected
//#define LED_GREEN   12
//#define LED_BLUE    13
//
//const char* mdnsName = "esp8266"; // Domain name for the mDNS responder
//void setupESP8266(){
//  
//  startWiFi();                 // Start a Wi-Fi access point, and try to connect to some given access points. Then wait for either an AP or STA connection
//  
////  startOTA();                  // Start the OTA service
//  
////  startSPIFFS();               // Start the SPIFFS and list all contents
//
//  startWebSocket();            // Start a WebSocket server
//  
////  startMDNS();                 // Start the mDNS responder
//
//  startServer();               // Start a HTTP server with a file read handler and an upload handler
//  
//}
//
//void startWiFi() { // Start a Wi-Fi access point, and try to connect to some given access points. Then wait for either an AP or STA connection
//  WiFi.softAP(ssid, password);             // Start the access point
//  Serial.print("Access Point \"");
//  Serial.print(ssid);
//  Serial.println("\" started\r\n");
//
//  wifiMulti.addAP("ssid_from_AP_1", "your_password_for_AP_1");   // add Wi-Fi networks you want to connect to
//  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
//  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");
//
//  Serial.println("Connecting");
//  while (wifiMulti.run() != WL_CONNECTED && WiFi.softAPgetStationNum() < 1) {  // Wait for the Wi-Fi to connect
//    delay(250);
//    Serial.print('.');
//  }
//  Serial.println("\r\n");
//  if(WiFi.softAPgetStationNum() == 0) {      // If the ESP is connected to an AP
//    Serial.print("Connected to ");
//    Serial.println(WiFi.SSID());             // Tell us what network we're connected to
//    Serial.print("IP address:\t");
//    Serial.print(WiFi.localIP());            // Send the IP address of the ESP8266 to the computer
//  } else {                                   // If a station is connected to the ESP SoftAP
//    Serial.print("Station connected to ESP8266 AP");
//  }
//  Serial.println("\r\n");
//}
////
////void startOTA() { // Start the OTA service
////  ArduinoOTA.setHostname(OTAName);
////  ArduinoOTA.setPassword(OTAPassword);
////
////  ArduinoOTA.onStart([]() {
////    Serial.println("Start");
////    digitalWrite(LED_RED, 0);    // turn off the LEDs
////    digitalWrite(LED_GREEN, 0);
////    digitalWrite(LED_BLUE, 0);
////  });
////  ArduinoOTA.onEnd([]() {
////    Serial.println("\r\nEnd");
////  });
////  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
////    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
////  });
////  ArduinoOTA.onError([](ota_error_t error) {
////    Serial.printf("Error[%u]: ", error);
////    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
////    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
////    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
////    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
////    else if (error == OTA_END_ERROR) Serial.println("End Failed");
////  });
////  ArduinoOTA.begin();
////  Serial.println("OTA ready\r\n");
////}
////
////void startSPIFFS() { // Start the SPIFFS and list all contents
////  SPIFFS.begin();                             // Start the SPI Flash File System (SPIFFS)
////  Serial.println("SPIFFS started. Contents:");
////  {
////    Dir dir = SPIFFS.openDir("/");
////    while (dir.next()) {                      // List the file system contents
////      String fileName = dir.fileName();
////      size_t fileSize = dir.fileSize();
////      Serial.printf("\tFS File: %s, size: %s\r\n", fileName.c_str(), formatBytes(fileSize).c_str());
////    }
////    Serial.printf("\n");
////  }
////}
//
//void startWebSocket() { // Start a WebSocket server
//  webSocket.begin();                          // start the websocket server
//  webSocket.onEvent(webSocketEvent);          // if there's an incomming websocket message, go to function 'webSocketEvent'
//  Serial.println("WebSocket server started.");
//}
//
////void startMDNS() { // Start the mDNS responder
////  MDNS.begin(mdnsName);                        // start the multicast domain name server
////  Serial.print("mDNS responder started: http://");
////  Serial.print(mdnsName);
////  Serial.println(".local");
////}
//
//void startServer() { // Start a HTTP server with a file read handler and an upload handler
//  server.on("/edit.html",  HTTP_POST, []() {  // If a POST request is sent to the /edit.html address,
//    server.send(200, "text/plain", ""); 
//  }, handleFileUpload);                       // go to 'handleFileUpload'
//
//  server.onNotFound(handleNotFound);          // if someone requests any other file or page, go to function 'handleNotFound'
//                                              // and check if the file exists
//
//  server.begin();                             // start the HTTP server
//  Serial.println("HTTP server started.");
//}
//

