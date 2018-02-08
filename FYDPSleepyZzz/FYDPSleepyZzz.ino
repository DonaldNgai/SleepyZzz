#include <SparkFun_ADXL345.h>
#include <Filter.h>
#include <SoftwareSerial.h>
#include "RollingAverage.h"
#include "HeartSpeed.h"

#define LOOP_DURATION_IN_MILLIS  1000
#define SERIAL_BAUD_RATE 9600
#define AVERAGING_BUFFER_SIZE 10
#define AVERAGE_HEART_RATE_BUFFER_SIZE 3
#define DATA_STORAGE_DEPTH 12

typedef enum
{
  ACCEL_X,
  ACCEL_Y,
  ACCEL_Z
} accel_orientation;

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
  setupADXL345();

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
      freefall_data[current_index] = freefall_detected;

      //if (current_index == DATA_STORAGE_DEPTH) 
      //PUT CODE TO SEND DATA HERE
      
      //Send Data Package
      ADXL_ISR(&freefall_detected);

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
