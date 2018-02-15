#include <SparkFun_ADXL345.h>
#include <Filter.h>
#include <SoftwareSerial.h>
#include "RollingAverage.h"
#include "HeartSpeed.h"
#include <avr/pgmspace.h>
#include "ArduinoJson-v5.13.0.h"

#define LOOP_DURATION_IN_MILLIS  500
#define SERIAL_BAUD_RATE 9600
#define AVERAGING_BUFFER_SIZE 5
#define AVERAGE_HEART_RATE_BUFFER_SIZE 5
#define DATA_STORAGE_DEPTH 5

typedef enum
{
  ACCEL_X,
  ACCEL_Y,
  ACCEL_Z
} accel_orientation;

//-------------Data Request Variables----------//
//String token = "";
//char* tokenssss;
//char* token = new char[270];
//const char* jsonToken;
//char* jsonToken = new char[211];
static char token[270] = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJkZXZpY2UiOiItS3BlUlBqZ2hlYmM1WWpYeUZTZiIsImJhYnkiOiItS3BlZGdVZkRPU1AwZFRnb0ZQMCIsImlhdCI6MTUxODExODUwNywiZXhwIjoxNTE4MjA0OTA3fQ.wts8u_hzJ9WbGbx4FJ2ToGCyVaA7GNCmmYDxaLAPRFY";
//char * token = new char[270];
char * data = new char[300];
char * val = new char[20];

#define FLOAT_STRING_LENGTH 7
#define FLOAT_STRING_DECIMALS 2

//-------------Sensor Variable Initialization----------//
SoftwareSerial esp8266(2,3); //RX - pin 2, TX - pin 3
const int heartPin = A1; 
int tempPin = 0;
ADXL345 adxl = ADXL345();             // USE FOR I2C COMMUNICATION

//--------------Temperature Sensor Variables------------//
float celsiusTemp;
float averagedTemp;
float temp_data [DATA_STORAGE_DEPTH];
RollingAverage TempAverage(AVERAGING_BUFFER_SIZE);

//--------------Heart Rate Sensor Variables-------------//
float averagedHeartRate;
float heart_rate_data [DATA_STORAGE_DEPTH];
HeartSpeed heartspeed(heartPin); 
RollingAverage HeartRateAverage(AVERAGE_HEART_RATE_BUFFER_SIZE);

//--------------Accerlerometer Variables----------------//
int x,y,z;
float averagedX,averagedY,averagedZ;
float accel_data [3][DATA_STORAGE_DEPTH];
//RollingAverage XAverage = new RollingAverage(AVERAGING_BUFFER_SIZE);
//RollingAverage YAverage = new RollingAverage(AVERAGING_BUFFER_SIZE);
//RollingAverage ZAverage = new RollingAverage(AVERAGING_BUFFER_SIZE);
RollingAverage XAverage(AVERAGING_BUFFER_SIZE);
RollingAverage YAverage(AVERAGING_BUFFER_SIZE);
RollingAverage ZAverage(AVERAGING_BUFFER_SIZE);
bool freefall_detected;
bool freefall_data [DATA_STORAGE_DEPTH];
#define offsetX   0.5       // OFFSET values
#define offsetY   6.5
#define offsetZ   15.5

#define gainX     -255.5        // GAIN factors
#define gainY     -254.5
#define gainZ     -251.5 

//--------------Global Variables------------//
unsigned long int startTime = 0;
short int current_index = 0;

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  esp8266.begin(SERIAL_BAUD_RATE);

  connectWifi();

  Serial.println(F("\r\nFinished Connecting\r\n"));

  while (!tokenRequest());
  
  delay(100);
//  int index = token.indexOf("token");
  Serial.println(F(""));
  Serial.println(F("Token: "));
//  Serial.println(index);
//  Serial.println(token.substring(50,80));
  Serial.println(token);
//
//  const size_t capacity = JSON_OBJECT_SIZE(2) + 286;
//  DynamicJsonBuffer jsonBuffer(capacity);
//  JsonObject& root = jsonBuffer.parseObject(token);
  
  setupADXL345();

  heartspeed.setCB(heartrate_cb);
  heartspeed.begin();

  Serial.println("Setup Complete");
}

void loop() {
// ////Uncomment this to interface with the wifi module
//  while(true){
//    if ( esp8266.available() )   {  
//      Serial.write(esp8266.read());
//    }
//
//    if ( Serial.available() )   {  
//      esp8266.write(Serial.read());
//    }
//  }
  
    if (millis() - startTime > LOOP_DURATION_IN_MILLIS){
      startTime = millis();

      temp_data[current_index] = averagedTemp;
      heart_rate_data[current_index] = averagedHeartRate;
      accel_data[ACCEL_X][current_index] = averagedX;
      accel_data[ACCEL_Y][current_index] = averagedY;
      accel_data[ACCEL_Z][current_index] = averagedZ;
      ADXL_ISR(&freefall_detected);
      freefall_data[current_index] = freefall_detected;
      
      if (current_index == DATA_STORAGE_DEPTH-1) {
          heartspeed.stop();
          sendData(heart_rate_data,accel_data,freefall_data,temp_data);

//          while (true){
//              char output[100];
//              short output_index = 0;
//              Serial.flush();
//              while(esp8266.available()){
//                  char c = esp8266.read();
////                Serial.write(esp8266.read());
//                  output[output_index] = c;
//                  output_index++;
//                  output[output_index] = "\0";
////                  Serial.print(c);
//              }
//              Serial.println("HI");
//              Serial.print(output);
//              while(!esp8266.available()){}     
//          }

          readResponse(10000);

          //This is necessary to ensure that the Serial is done writing before enabling
          //Interrupts to ensure that writes aren't interrupted.
          delay(200); 
          
          heartspeed.begin();
      }

//      Serial.print("Index: ");
//      Serial.print(current_index);
//      Serial.print(" Temp: ");
//      Serial.print(averagedTemp);
      Serial.print(" XR: ");
      Serial.print(x);
      Serial.print(" YR: ");
      Serial.print(y);
      Serial.print(" ZR: ");
      Serial.println(z);
      Serial.print(" X: ");
      Serial.print(averagedX);
      Serial.print(" Y: ");
      Serial.print(averagedY);
      Serial.print(" Z: ");
      Serial.println(averagedZ);
//      Serial.print(" Free: ");
//      Serial.print(freefall_detected);
//      Serial.print(" Heart: ");
//      Serial.println(averagedHeartRate);

      current_index = (current_index + 1) % DATA_STORAGE_DEPTH;
    }
    
    adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z
//    Serial.println(F("\r\n\r\nX"));
    XAverage.add_value((x - offsetX)/gainX,&averagedX);
//    Serial.println(F("\r\n\r\nY"));
    YAverage.add_value((y - offsetY)/gainY,&averagedY);
//    Serial.println(F("\r\n\r\nZ"));
    ZAverage.add_value((z - offsetZ)/gainZ,&averagedZ);

    get_temp_in_celsius(&celsiusTemp);
//    Serial.println(F("\r\n\r\nTemp"));
    TempAverage.add_value(celsiusTemp,&averagedTemp);
        
}
