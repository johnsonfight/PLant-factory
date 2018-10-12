void communicateWithPC() {
  int readingBytes = Serial.available(); // return reading bytes
    
  if (readingBytes > 1) {
    char readHeader = Serial.read(); // read the header
    char value[readingBytes]; // store codes from GUI
      
    // Control Plant
    if (readHeader == 'N') { 
      for (int i = 0; i < readingBytes - 1; i++)
        value[i] = Serial.read();
        
      double valueForPrint = atof(value); // string to double
      Serial.print('N');
      Serial.println(valueForPrint);

      if (controlOrNot == false){ 
        if (value[0] == '1') { // light1
          if (value[1] == '0')
            digitalWrite(light1 , LOW);
          else if (value[1] == '1')
            digitalWrite(light1, HIGH);
        }

        else if (value[0] == '2') { // light2
          if (value[1] == '0')
            digitalWrite(light2 , LOW);
          else if (value[1] == '1')
            digitalWrite(light2, HIGH);
        }

        else if (value[0] == '3') { // light3
          if (value[1] == '0')
            digitalWrite(light3 , LOW);
          else if (value[1] == '1')
            digitalWrite(light3, HIGH);
        }

        else if (value[0] == '4') { // CO2
          if (value[1] == '0')
            digitalWrite(CO2, LOW);
          else if (value[1] == '1')
            digitalWrite(CO2, HIGH);
        }

        else if (value[0] == '5') { // Heater
          if (value[1] == '0')
            digitalWrite(heater , LOW);
          else if (value[1] == '1')
            digitalWrite(heater, HIGH);
        }
        
        else if (value[0] == '6') { // Humidifier
          if (value[1] == '0')
            digitalWrite(humidifier , LOW);
          else if (value[1] == '1')
            digitalWrite(humidifier, HIGH);
        }

        else if (value[0] == '7') { // Heatpump
          if (value[1] == '0')
            digitalWrite(heatpump , LOW);
          else if (value[1] == '1')
            digitalWrite(heatpump, HIGH);
        }
      }
    }
      
    // Start and Stop
    if (readHeader == 'S') {
      // tem target value
      for (int i = 0; i < readingBytes - 2; i++)
        value[i] = Serial.read();
      double lightTemValue = atof(value); 
        
      if (value[0] == '1') { // start control
        Serial.println("S,1");
        controlOrNot = true;
        timer = 0;
        co2Time = co2DelayTime;
        count = 0;
        lightNowTarget = lightTarget[count];
        co2NowTarget = co2Target[count];
        rhNowTarget = rhTarget[count];
        temNowTarget = temTarget[count];
        
        Serial.print("T");  // return start targets
        Serial.print(",");
        Serial.println(0);
          
        ++count;
      }
      else if (value[0] == '0') { // not control
        Serial.println("S,0");
        controlOrNot = false;
      }
    }
    


    // setup temperature targets
    else if (readHeader == 'T'){
      // Nth tem target
      char N = Serial.read();

      // tem target value
      for (int i = 0; i < readingBytes - 2; i++)
        value[i] = Serial.read();
      double lightTemValue = atof(value); 

      if (controlOrNot == false){
        if (N == '0')
          temTarget[0] = lightTemValue;
        else if (N == '1')
          temTarget[1] = lightTemValue;
        else if (N == '2')
          temTarget[2] = lightTemValue;
        else if (N == '3')
          temTarget[3] = lightTemValue;
        else if (N == '4')
          temTarget[4] = lightTemValue;
        else if (N == '5')
          temTarget[5] = lightTemValue;
        else if (N == '6')
          temTarget[6] = lightTemValue;
        else if (N == '7')
          temTarget[7] = lightTemValue;
        else if (N == '8')
          temTarget[8] = lightTemValue;
        else if (N == '9')
          temTarget[9] = lightTemValue;
        else if (N == 'A')
          temTarget[10] = lightTemValue;
        else if (N == 'B')
          temTarget[11] = lightTemValue;
        else if (N == 'C')
          temTarget[12] = lightTemValue;
        else if (N == 'D')
          temTarget[13] = lightTemValue;
        else if (N == 'E'){
          temTarget[14] = lightTemValue;
          temTarget[15] = lightTemValue;
        }
      }
    }    
    
    
    
    // setup RH targets 
    else if (readHeader == 'W'){
      // Nth RH target
      char N = Serial.read();

      // RH target value
      for (int i = 0; i < readingBytes - 2; i++)
        value[i] = Serial.read();
      double rhTargetValue = atof(value); 
        
      if (controlOrNot == false){
        if (N == '0')
          rhTarget[0] = rhTargetValue;
        else if (N == '1')
          rhTarget[1] = rhTargetValue;
        else if (N == '2')
          rhTarget[2] = rhTargetValue;
        else if (N == '3')
          rhTarget[3] = rhTargetValue;
        else if (N == '4')
          rhTarget[4] = rhTargetValue;
        else if (N == '5')
          rhTarget[5] = rhTargetValue;
        else if (N == '6')
          rhTarget[6] = rhTargetValue;
        else if (N == '7')
          rhTarget[7] = rhTargetValue;
        else if (N == '8')
          rhTarget[8] = rhTargetValue;
        else if (N == '9')
          rhTarget[9] = rhTargetValue;
        else if (N == 'A')
          rhTarget[10] = rhTargetValue;
        else if (N == 'B')
          rhTarget[11] = rhTargetValue;
        else if (N == 'C')
          rhTarget[12] = rhTargetValue;
        else if (N == 'D')
          rhTarget[13] = rhTargetValue;
        else if (N == 'E'){
          rhTarget[14] = rhTargetValue;
          rhTarget[15] = rhTargetValue;
          rhTarget[16] = rhTargetValue;
        }
      }
    }



    // setup Co2 targets
    else if (readHeader == 'C'){
      // Nth co2 target
      char N = Serial.read();
        
      // co2 target value
      for (int i = 0; i < readingBytes - 2; i++) 
        value[i] = Serial.read();
      int co2TargetValue = atof(value);
        
      if (controlOrNot == false){
        if (N == '0')
          co2Target[0] = co2TargetValue;
        else if (N == '1')
          co2Target[1] = co2TargetValue;
        else if (N == '2')
          co2Target[2] = co2TargetValue;
        else if (N == '3')
          co2Target[3] = co2TargetValue;
        else if (N == '4')
          co2Target[4] = co2TargetValue;
        else if (N == '5')
          co2Target[5] = co2TargetValue;
        else if (N == '6')
          co2Target[6] = co2TargetValue;
        else if (N == '7')
          co2Target[7] = co2TargetValue;
        else if (N == '8')
          co2Target[8] = co2TargetValue;
        else if (N == '9')
          co2Target[9] = co2TargetValue;
        else if (N == 'A')
          co2Target[10] = co2TargetValue;
        else if (N == 'B')
          co2Target[11] = co2TargetValue;
        else if (N == 'C')
          co2Target[12] = co2TargetValue;
        else if (N == 'D')
          co2Target[13] = co2TargetValue;
        else if (N == 'E'){
          co2Target[14] = co2TargetValue;
          co2Target[15] = co2TargetValue;
        }
      }
    }
 
 
    // setup Light targets
    else if (readHeader == 'L'){
      // Nth light target
      char N = Serial.read();

      // light target value
      for (int i = 0; i < readingBytes - 2; i++)
        value[i] = Serial.read();
      int lightTargetValue = atof(value); 
        
      if (controlOrNot == false){
        if (N == '0')
          lightTarget[0] = lightTargetValue;
        else if (N == '1')
          lightTarget[1] = lightTargetValue;
        else if (N == '2')
          lightTarget[2] = lightTargetValue;
        else if (N == '3')
          lightTarget[3] = lightTargetValue;
        else if (N == '4')
          lightTarget[4] = lightTargetValue;
        else if (N == '5')
          lightTarget[5] = lightTargetValue;
        else if (N == '6')
          lightTarget[6] = lightTargetValue;
        else if (N == '7')
          lightTarget[7] = lightTargetValue;
        else if (N == '8')
          lightTarget[8] = lightTargetValue;
        else if (N == '9')
          lightTarget[9] = lightTargetValue;
        else if (N == 'A')
          lightTarget[10] = lightTargetValue;
        else if (N == 'B')
          lightTarget[11] = lightTargetValue;
        else if (N == 'C')
          lightTarget[12] = lightTargetValue;
        else if (N == 'D')
          lightTarget[13] = lightTargetValue;
        else if (N == 'E'){
          lightTarget[14] = lightTargetValue;
          lightTarget[15] = lightTargetValue;
          
          
          Serial.print("C"); // return to check
          for(int i=0; i<15; ++i) {
            Serial.print(",");
            Serial.print(temTarget[i]);
            Serial.print(",");
            Serial.print(rhTarget[i]);
            Serial.print(",");
            Serial.print(co2Target[i]);
            Serial.print(",");
            Serial.print(lightTarget[i]);
          }
          Serial.print("\n");
        }
      }
    }


  }
}
