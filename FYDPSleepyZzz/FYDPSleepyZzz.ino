#include <SparkFun_ADXL345.h>
#include <Filter.h>
#include <SoftwareSerial.h>
#include "RollingAverage.h"
#include "HeartSpeed.h"
#include <avr/pgmspace.h>

#define LOOP_DURATION_IN_MILLIS  1000
#define SERIAL_BAUD_RATE 9600
#define AVERAGING_BUFFER_SIZE 10
#define AVERAGE_HEART_RATE_BUFFER_SIZE 3
#define DATA_STORAGE_DEPTH 3

typedef enum
{
  ACCEL_X,
  ACCEL_Y,
  ACCEL_Z
} accel_orientation;

//-------------Data Request Variables----------//
//char header1[] = "POST /api/v1/data?token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJkZXZpY2UiOiItS3BlUlBqZ2hlYmM1WWpYeUZTZiIsImJhYnkiOiItS3BlZGdVZkRPU1AwZFRnb0ZQMCIsImlhdCI6MTUxODExODUwNywiZXhwIjoxNTE4MjA0OTA3fQ.wts8u_hzJ9WbGbx4FJ2ToGCyVaA7GNCmmYDxaLAPRFY HTTP/1.1\r\nHost: 138.197.153.154:80\r\nContent-Type: application/json\r\nContent-Length: ";
//char header2[] = "\r\nUser-Agent: Arduino/1.0\r\nAuthorization: Basic c2xlZXB5enp6OkRFNEYxQzE3LTMwOEQtNEY0OS04MjU2LTRERTlFN0M5QjhDQg==\r\nConnection: Close\r\n\r\n";
//const char string_0[] PROGMEM = "POST /api/v1/data?token=";
////token
//const char string_1[] PROGMEM = " HTTP/1.1\r\nHost: 138.197.153.154:80\r\nContent-Type: ";
//const char string_2[] PROGMEM = "application/json\r\nContent-Length: ";
////content length
//const char string_3[] PROGMEM = "\r\nUser-Agent: Arduino/1.0\r\nAuthorization:";
//const char string_4[] PROGMEM = " Basic c2xlZXB5enp6OkRFNEYxQzE3LTMwOEQtNEY0OS";
//const char string_5[] PROGMEM = "04MjU2LTRERTlFN0M5QjhDQg==\r\nConnection: Close\r\n\r\n";
//const char* const string_table[] PROGMEM = {string_0, string_1, string_2, string_3, string_4, string_5};
//char header1[330];
//char header2[150];
//char header[330];

char * data = new char[300];
char * val = new char[20];
String s = "";

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
//int heartValue;
//float SmoothHeart;
////// the <float> makes a filter for float numbers
////// 20 is the weight (20 => 20%)
////// 0 is the initial value of the filter
//ExponentialFilter<float> FilteredHeart(20, 0);
//float lp1, lp2 = 0;
//float lpfreq = 0.10;
//float hpfreq = 0.20;

//--------------Accerlerometer Variables----------------//
int x,y,z;
float averagedX,averagedY,averagedZ;
float accel_data [3][DATA_STORAGE_DEPTH];
RollingAverage XAverage(AVERAGING_BUFFER_SIZE);
RollingAverage YAverage(AVERAGING_BUFFER_SIZE);
RollingAverage ZAverage(AVERAGING_BUFFER_SIZE);
//Accelerometer values are more prone to noise so I want a stronger filter
// the <float> makes a filter for float numbers
// 20 is the weight (20 => 20%)
// 0 is the initial value of the filter
//ExponentialFilter<int> x_filter(20, 0); 
//ExponentialFilter<int> y_filter(20, 0); 
//ExponentialFilter<int> z_filter(20, 0);
//int filtered_x, filtered_y, filtered_z;
bool freefall_detected;
bool freefall_data [DATA_STORAGE_DEPTH];

//--------------Global Variables------------//
unsigned long int startTime = 0;
short int current_index = 0;

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  esp8266.begin(SERIAL_BAUD_RATE);
  setupADXL345();
//
  heartspeed.setCB(heartrate_cb);
  heartspeed.begin();
}

void loop() {
  
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

          //This is necessary to ensure that the Serial is done writing before enabling
          //Interrupts to ensure that writes aren't interrupted.
          delay(100); 
          
          heartspeed.begin();
      }

//      Serial.print("Index: ");
//      Serial.print(current_index);
//      Serial.print(" Temp: ");
//      Serial.print(averagedTemp);
//      Serial.print(" X: ");
//      Serial.print(averagedX);
//      Serial.print(" Y: ");
//      Serial.print(averagedY);
//      Serial.print(" Z: ");
//      Serial.print(averagedZ);
//      Serial.print(" Free: ");
//      Serial.print(freefall_detected);
//      Serial.print(" Heart: ");
//      Serial.println(averagedHeartRate);

      current_index = (current_index + 1) % DATA_STORAGE_DEPTH;
    }
    
    adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z
    XAverage.add_value(x,&averagedX);
    YAverage.add_value(y,&averagedY);
    ZAverage.add_value(z,&averagedZ);

    get_temp_in_celsius(&celsiusTemp);
    TempAverage.add_value(celsiusTemp,&averagedTemp);
        
}
