#include "RollingAverage.h"

RollingAverage::RollingAverage(int numReadings)
{
  _numReadings = numReadings;
  readings = new int[numReadings];
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

  // calculate the average:
  *new_average = total / _numReadings;
}

