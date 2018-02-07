#include <SparkFun_ADXL345.h>
#include <Filter.h>
#include <SoftwareSerial.h>
#include "RollingAverage.h"

#define LOOP_DURATION_IN_MILLIS  1000
#define SERIAL_BAUD_RATE 9600
#define AVERAGING_BUFFER_SIZE 10

//-------------Sensor Variable Initialization----------//
SoftwareSerial esp8266(2,3); //RX - pin 2, TX - pin 3
const int heartPin = A1; 
int tempPin = 0;
ADXL345 adxl = ADXL345();             // USE FOR I2C COMMUNICATION

//--------------Temperature Sensor Variables------------//
float celsiusTemp, averagedTemp;
RollingAverage TempAverage(AVERAGING_BUFFER_SIZE);

//--------------Heart Rate Sensor Variables-------------//
int heartValue;
float SmoothHeart;
// the <float> makes a filter for float numbers
// 20 is the weight (20 => 20%)
// 0 is the initial value of the filter
ExponentialFilter<float> FilteredHeart(20, 0);
float lp1, lp2 = 0;
float lpfreq = 0.10;
float hpfreq = 0.20;

//--------------Accerlerometer Variables----------------//
int x,y,z;
//Accelerometer values are more prone to noise so I want a stronger filter
// the <float> makes a filter for float numbers
// 20 is the weight (20 => 20%)
// 0 is the initial value of the filter
ExponentialFilter<int> x_filter(20, 0); 
ExponentialFilter<int> y_filter(20, 0); 
ExponentialFilter<int> z_filter(20, 0);
int filtered_x, filtered_y, filtered_z;
bool freefall_detected;

//--------------Global Variables------------//
unsigned long int startTime = 0;

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  setupADXL345();
}

void loop() {
  
    if (millis() - startTime > LOOP_DURATION_IN_MILLIS){
      startTime = millis();
      //Send Data Package
    }
    adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z
    filter_adxl(&x, &y, &z, &x_filter, &y_filter, &z_filter, &filtered_x, &filtered_y, &filtered_z); 

    get_temp_in_celsius(&celsiusTemp);
    TempAverage.add_value(celsiusTemp,&averagedTemp);
    
    //ADXL_ISR(&freefall_detected);
    //Package data
    
}
