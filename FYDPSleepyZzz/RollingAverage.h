#ifndef RollingAverage_h
#define RollingAverage_h

class RollingAverage
{
  public:
    RollingAverage(int numReadings);
    ~RollingAverage();
    void add_value(float new_value, float* new_average);
  private:
    int   _numReadings;
    float*  readings;                  // the readings from the analog input
    int   readIndex = 0;              // the index of the current reading
    float total = 0;                  // the running total
    float average = 0;                // the average
};

#endif
