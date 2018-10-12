void control() {
  if (controlOrNot == true) {
    // set new targets
    if (timer % 60 == 53 && count < 16)
      lightNowTarget = lightTarget[count - 1];
    if (timer % 60 == 58 && count < 16) {
      co2NowTarget = co2Target[count];
      rhNowTarget = rhTarget[count];
      temNowTarget = temTarget[count];

      Serial.print("T"); // set red line (new target bar)
      Serial.print(",");
      if (count <= 5)
        Serial.println(0);
      else
        Serial.println(count - 5);

      if (rhTarget[count + 1] > rhTarget[count] && rhTarget[count] < rhTarget[count - 1])
        rhNowTarget = (rhTarget[count] * 3 + rhTarget[count + 1]) / 4;
      else if (rhTarget[count + 1] < rhTarget[count] && rhTarget[count] > rhTarget[count - 1])
        rhNowTarget = (rhTarget[count] * 3 + rhTarget[count + 1]) / 4;

      ++count;
    }

    //濕度特性參數
    coolTime = 33 + (7.0 * (1.0 + 0.1 * (28.0 - tem))) * (50.0 - rhNowTarget) / 10.0;

    if (heatpumpMark3 == 1)
      coolTime = coolTime * 1.5 + 5;
    coolPoint = 4 + 3 * (rhNowTarget - 50) / 10;
    deltaRhLowerLimit = 3 - rhNowTarget / 10;

    if (deltaRhLowerLimit > -1) deltaRhLowerLimit = -1;
    if (coolTime < 15) coolTime = 15;


    if (coolPoint > 9) coolPoint = 9;
    if (coolPoint < 4) coolPoint = 4;

    //濕度特性參數
    deltaCo2 = co2NowTarget - co2;
    deltaRh = rhNowTarget - rh;
    deltaTem = temNowTarget - tem;

    //二氧化碳控制開始
    if (timer % 30 == 1)
      co2Time = co2DelayTime;
    if (timer % 30 == 1 && timer >= 360) {
      co2Time = co2DelayTime + 0.3 * co2DelayTime * (co2Target[count - 1] - co2Target[count - 2]) / 100;
      if (co2Time < co2DelayTime) co2Time = co2DelayTime;
    }

    if (co2Time > 0) {
      if (deltaCo2 > deltaCo2Limit) {
        CO2Open = 1;
        --co2Time;
      }
      else
        CO2Open = 0;
    }
    else if (co2Time == 0)
      CO2Open = 0;

    //溫度濕度控制開始
    //溫度濕度情況判斷
    if (deltaTem >= deltaTemUpperLimit) {
      heaterOpen = 1;
      ++heaterTime;
    }
    else {
      heaterOpen = 0;
      heaterTime = 0;
    }
    if (deltaRh > 0) {
      humidifierOpen = 1;
      humidifierTime ++;
    }
    if (deltaRh <= deltaRhLowerLimit)
    {
      heatpumpOpen = 1;
      heatpumpTime ++;
      heatpumpMark = 1;
      if (deltaRh < -11)
        heatpumpMark2 = 1;
    }
    else if (deltaRh < 0 && heatpumpMark == 0) {
      heatpumpOpen = 1;
      heatpumpTime ++;
    }
    else if (deltaRh > deltaRhLowerLimit && heatpumpMark == 1)
      heatpumpOpen = 0;
    else {
      heatpumpOpen = 0;
      heatpumpTime = 0;
    }
    if (deltaRh > 5) heatpumpMark3 = 1;

    //減緩濕度震盪
    if (deltaRh >= 0) {
      heatpumpOpen = 0;
      heatpumpMark = 0;
      heatpumpMark2 = 0;
    }
    if (deltaRh > -5) heatpumpMark2 = 0;

    //根據變數接近狀態修正
    if (heatpumpTime > 0) {
      if (heatpumpTime > coolTime && heatpumpTime % 4 > 1  && deltaRh > -10) {
        heatpumpOpen = 1;
        heatpumpMark = 1;
      }
      else if (heatpumpTime > coolTime && heatpumpTime % 4 < 2  && deltaRh > -10) heatpumpOpen = 0;
      else if (heatpumpTime <= coolTime) {
        heatpumpOpen = 1;
        heatpumpMark3 = 0;
        if (deltaTem >= 0)
          if (timer % 4 < 2 && deltaTem > 0.3)
            heaterOpen = 1;
      }
      else if (heatpumpTime > coolTime && heatpumpTime % 4 > 0) {
        heatpumpOpen = 1;
        heatpumpMark = 1;
      }
      else if (heatpumpTime > coolTime && heatpumpTime % 4 < 1)
        heatpumpOpen = 0;
      humidifierOpen = 0;
    }
    else
      humidifierOpen = 1;
    if (heaterTime > 0) {
      if (heaterTime > 10) {
        if (heaterTime % 10 < 5 && deltaTem < 0.9) heaterOpen = 0;
        if (heaterTime % 10 < 8 && deltaTem < 0.7) heaterOpen = 0;
        if (deltaTem > 1) heaterOpen = 1;
      }
    }

    // 濕度優先
    if (deltaRh > 0) {
      heatpumpOpen = 0;
      heatpumpTime = 0;
    }

    if (deltaRh > deltaRhLowerLimit && heatpumpMark == 1) {
      heatpumpOpen = 0;
      heatpumpTime = 0;
    }
    if (heatpumpMark2 == 1 && deltaRh > 0 - coolPoint)
      heatpumpOpen = 0;

    // 調整初始狀態
    if (timer < 120) {
      if (deltaTem < deltaTemLowerLimit) {
        heatpumpOpen = 1;
        pumpTime ++;
      }
      else
        pumpTime = 0;
      if (pumpTime > 0) {
        if (pumpTime > 10) {
          if (pumpTime % 10 < 5 && deltaTem > -0.3 && deltaTem >= 0) heaterOpen = 1;
          if (deltaRh < -2 && pumpTime % 10 > 5) humidifierOpen = 1;
          if (pumpTime % 10 < 5 && deltaTem > -0.6) heatpumpOpen = 0;
        }
      }
    }
    else
      pumpTime = 0;

    // light control
    if (timer % 60 > 53)
      lightControl(lightNowTarget);
    else if (timer % 60 == 6)
      lightControl(000);
      
      
    implement();
    ++timer;
  }
}

// light control
void lightControl(int lightTarget) {
  if (lightTarget == 000) {
    light1Open = 0;
    light2Open = 0;
    light3Open = 0;
  }
  else if (lightTarget == 1) {
    light1Open = 0;
    light2Open = 0;
    light3Open = 1;
  }
  else if (lightTarget == 10) {
    light1Open = 0;
    light2Open = 1;
    light3Open = 0;
  }
  else if (lightTarget == 100) {
    light1Open = 1;
    light2Open = 0;
    light3Open = 0;
  }
  else if (lightTarget == 11) {
    light1Open = 0;
    light2Open = 1;
    light3Open = 1;
  }
  else if (lightTarget == 110) {
    light1Open = 1;
    light2Open = 1;
    light3Open = 0;
  }
  else if (lightTarget == 101) {
    light1Open = 1;
    light2Open = 0;
    light3Open = 1;
  }
  else if (lightTarget == 111) {
    light1Open = 1;
    light2Open = 1;
    light3Open = 1;
  }
}

void implement() {
  
  if (light1Open == 1)
    digitalWrite(light1, HIGH);
  else if (light1Open == 0)
    digitalWrite(light1, LOW);
    
  if (light2Open == 1)
    digitalWrite(light2, HIGH);
  else if (light2Open == 0)
    digitalWrite(light2, LOW);
    
  if (light3Open == 1)
    digitalWrite(light3, HIGH);
  else if (light3Open == 0)
    digitalWrite(light3, LOW);
  
  if (CO2Open == 1)
    digitalWrite(CO2, HIGH);
  else if (CO2Open == 0)
    digitalWrite(CO2, LOW);
  
  if (heaterOpen == 1)
    digitalWrite(heater, HIGH);
  else if (heaterOpen == 0)
    digitalWrite(heater, LOW);
  
  if (humidifierOpen == 1)
    digitalWrite(humidifier, HIGH);
  else if (humidifierOpen == 0)
    digitalWrite(humidifier, LOW);

  if (heatpumpOpen == 1)
    digitalWrite(heatpump, HIGH);
  else if (heatpumpOpen == 0)
    digitalWrite(heatpump, LOW);
}
