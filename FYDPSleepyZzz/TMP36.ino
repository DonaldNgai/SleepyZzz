void get_temp_in_celsius(float* celsius_temp)
{
    short int rawTempVoltage = analogRead(tempPin); 
//    Serial.println(rawTempVoltage);
//    float tempMilliVolts = (rawTempVoltage * ((5000/1024)/1000));
    float tempMilliVolts = (rawTempVoltage * 0.0048828125);
    *celsius_temp = (tempMilliVolts - 0.5) * 100;

//    Serial.print("Raw: ");
//    Serial.print(rawTempVoltage);
//    Serial.print("Milli: ");
//    Serial.print(tempMilliVolts);
//    Serial.print(" Cels: ");
//    Serial.println(*celsius_temp);
}

