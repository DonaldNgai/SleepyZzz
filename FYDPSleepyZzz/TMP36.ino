void get_temp_in_celsius(float* celsius_temp)
{
    int rawTempVoltage = analogRead(tempPin); 
    float tempMilliVolts = (rawTempVoltage * ((5000/1024)/1000));
    *celsius_temp = (tempMilliVolts - 0.5) * 100;
}

