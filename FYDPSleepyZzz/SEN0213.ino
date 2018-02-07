void get_heart_rate(int* heart_rate){
    heartValue = analogRead(heartPin);
    FilteredHeart.Filter(heartValue);
    SmoothHeart = FilteredHeart.Current();
    lp1 = lp1 + lpfreq*(SmoothHeart-lp1);
    lp2 = lp2 + hpfreq*(SmoothHeart-lp1-lp2);

    Serial.print(heartValue);
    Serial.print(", ");
    Serial.print(SmoothHeart); 
    Serial.print(", ");
    Serial.println(lp2); 
}

