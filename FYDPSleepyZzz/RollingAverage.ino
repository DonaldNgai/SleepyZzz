#include "RollingAverage.h"

RollingAverage::RollingAverage(byte numReadings)
{
  _numReadings = numReadings;
  readings = new float[numReadings];
//  Serial.println(_numReadings);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
//    Serial.print(thisReading);
//    Serial.print(" ");
//    Serial.println(readings[thisReading]);
  }
}

RollingAverage::~RollingAverage()
{
  delete[] readings;
}

void RollingAverage::add_value(float new_value, float* new_average)
{
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = new_value;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= _numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  
//  for (int thisReading = 0; thisReading < _numReadings; thisReading++) {
////    readings[thisReading] = 0;
//    Serial.print(readings[thisReading]);
//    Serial.print(" ");
//  }
//

  // calculate the average:
  *new_average = total / _numReadings;
  if (*new_average > 1000){ 
    Serial.print(F("\nNew: "));
    Serial.print(new_value);
    Serial.print(F(" Total: "));
    Serial.print(total);
    Serial.print(F(" numReadings: "));
    Serial.print(_numReadings);
    Serial.print(F(" Avg: "));
    Serial.println(*new_average);
  }
}

