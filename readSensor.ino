void readSensor() {
  
  delay(20);
  // read from sensor
  if (Serial3.available() >= 15) {
    while(Serial3.read() != 02) {} // dummy
    Serial3.read();
    unsigned int data[6];
    for (int i = 0; i < 6; i++)
      data[i] = Serial3.read();

    // calculate reading
    co2 = data[0] * 256.0 + data[1];
    rh = (data[2] * 256.0 + data[3]) / 10.0;
    tem = (data[4] * 256.0 + data[5]) / 10.0;
    
    if(co2 <= 6000 && co2 >= 400)
      LastCo2 = co2;
    else
      co2 = LastCo2;
    if(rh <= 100 && rh >= 10)
      LastRh = rh;
    else
      rh = LastRh;
    if(tem <= 40 && tem >= 10)
      LastTem = tem;  
    else
      tem = LastTem;

    Serial.print("D");  // output to GUI LCD
    Serial.print(",");
    Serial.print(tem);
    Serial.print(",");
    Serial.print(rh);
    Serial.print(",");
    Serial.print(co2);
    
    Serial.print(","); // light1 icon
    if (light1Open == 1)
      Serial.print("1");
    else
      Serial.print("0");
      
    Serial.print(","); // light2 icon
    if (light2Open == 1)
      Serial.print("1");
    else
      Serial.print("0");
      
    Serial.print(","); // light3 icon
    if (light3Open == 1)
      Serial.print("1");
    else
      Serial.print("0");
    
    
    Serial.print(","); // CO2 icon
    if (CO2Open == 1)
      Serial.print("1");
    else
      Serial.print("0");
      
    Serial.print(","); // heater icon
    if (heaterOpen == 1)
      Serial.print("1");
    else
      Serial.print("0");
      
    Serial.print(","); // humidifier icon
    if (humidifierOpen == 1)
      Serial.print("1");
    else
      Serial.print("0");
      
    Serial.print(","); // heatpump icon
    if (heatpumpOpen == 1)
      Serial.println("1");
    else
      Serial.println("0");
  }
}
