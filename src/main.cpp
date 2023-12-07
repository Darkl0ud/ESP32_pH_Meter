/* Made by Darkloud using DIY-MORE PH-4502C pH controller board and used specifically
 on the ESP32-S3 DevKitC */

#include <main.h>

void IRAM_ATTR buttonUpPressed() {
  if (digitalRead(UP_BUTTON_PIN) == 1)  {
    bUpPressed = true;
  }
  else  {
    bUpPressed = false;
  }
}

void IRAM_ATTR buttonDownPressed() {
  if (digitalRead(DOWN_BUTTON_PIN) == 1)  {
    bDownPressed = true;
  }
  else  {
    bDownPressed = false;
  }
}

void IRAM_ATTR buttonLeftPressed() {
  if (digitalRead(LEFT_BUTTON_PIN) == 1)  {
    bLeftPressed = true;
  }
  else  {
    bLeftPressed = false;
  }
}

void IRAM_ATTR buttonRightPressed() {
  if (digitalRead(RIGHT_BUTTON_PIN) == 1)  {
    bRightPressed = true;
  }
  else  {
    bRightPressed = false;
  }
}

void setDeviceState(char state) {  // Sets various indicators depending upon system status.

  switch (state) {

  case 0: // Normal State
    neopixelWrite(RGB_BUILTIN,0,RGB_BRIGHTNESS,0);  // Set Status RGB Green
    cDevState = 0;
    break;
  case 1: // Fault State
    neopixelWrite(RGB_BUILTIN,RGB_BRIGHTNESS,0,0);  // Set Status RGB Red
    cDevState = 1;
    break;
  case 2: // Notification State
    neopixelWrite(RGB_BUILTIN,0,0,RGB_BRIGHTNESS);  // Set Status RGB Blue
    cDevState = 2;
    break;
  default:
    break;
  }
}

void uiHandler(){   // Handles UI IO
  switch (cSelectedMenuTab) {
  case 0:
    dUI.DisplayUpdate(0,fpHAverage,fpHTarget,0, bpHTimerEnded); // Update display with pH parameters.

    if (bUpPressed)
    {
      cSelectedMenuTab = 2;
      bUpPressed = false;
    }

    else if (bDownPressed)
    {
      cSelectedMenuTab = 1;
      bDownPressed = false;
    }
    break;
  
  case 1:
    dUI.DisplayUpdate(1,fpHAverage,fpHTarget,0, bpHTimerEnded); // Update display with EC parameters.

    if (bUpPressed)
    {
      cSelectedMenuTab = 0;
      bUpPressed = false;
    }
    
    else if (bDownPressed)
    {
      cSelectedMenuTab = 2;
      bDownPressed = false;
    }
    break;

  case 2:
    /* When Set tab is active... */
    if (bMenuTabActive) // If menu is active...
    {
      dUI.DisplayUpdate(2,fpHAverage,fpHTarget,1, bpHTimerEnded); // Update display with Setting parameters.

      if (bLeftPressed)
      {
        bMenuTabActive = false;
        bLeftPressed = false;
      }
      
      if (bUpPressed)
      {
        fpHTarget = fpHTarget + 0.1;
        bUpPressed = false;
      }
    
      else if (bDownPressed)
      {
        fpHTarget = fpHTarget - 0.1;
        bDownPressed = false;
      }
    }
    
    else  // If menu is not active
    {
      dUI.DisplayUpdate(2,fpHAverage,fpHTarget,0, bpHTimerEnded); // Update display with Setting parameters.

      if (bUpPressed)
      {
        cSelectedMenuTab = 1;
        bUpPressed = false;
      }
    
      else if (bDownPressed)
      {
        cSelectedMenuTab = 0;
        bDownPressed = false;
      }

      else if (bRightPressed)
      {
        bMenuTabActive = true;
        bRightPressed = false;
      }
    }
  default:
    break;
  }
}

void setup()  {
  Serial.begin(115200);
  setDeviceState(0);  // Set device state to 0, 'running'
  pinMode(PH_READ_PIN, INPUT);
  pinMode(UP_BUTTON_PIN, INPUT_PULLDOWN);
  pinMode(DOWN_BUTTON_PIN, INPUT_PULLDOWN);
  pinMode(LEFT_BUTTON_PIN, INPUT_PULLDOWN);
  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLDOWN);
  attachInterrupt(UP_BUTTON_PIN, buttonUpPressed, CHANGE);
  attachInterrupt(DOWN_BUTTON_PIN, buttonDownPressed, CHANGE);
  attachInterrupt(LEFT_BUTTON_PIN, buttonLeftPressed, CHANGE);
  attachInterrupt(RIGHT_BUTTON_PIN, buttonRightPressed, CHANGE);
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN); // Initialize Wire with custom SDA and SCL pins
  dUI.InitDisplay();
  dUI.DisplayUpdate(0,fpHAverage,fpHTarget,0, bpHTimerEnded); // Initial Display Update.
}

void calculateAveragepH() {
    if (lCurrentMillis - lLastpHMillis >= 100)  { // Once 100ms have passed...
    lLastpHMillis = lCurrentMillis;  // Save the current time.
    rawAnalog = analogRead(PH_READ_PIN) / 4095.0; // Read from pH probe. Gives an analog 0-1 value.
    float b;
    float pH;

    if (rawAnalog >= fpH14Cal && rawAnalog <= fpH10Cal) {  // pH 14 - 10
      b = 14 - ((14.0 - 10.0) / (fpH14Cal - fpH10Cal)) * fpH14Cal;  // Calculate Y Intercept based off of pH Calibration slope.
      pH = ((14.0 - 10.0) / (fpH14Cal - fpH10Cal)) * rawAnalog + b;
    }
    
    if (rawAnalog >= fpH10Cal && rawAnalog <= fpH7Cal)  {  // pH 10 - 7
      b = 10 - ((10.0 - 7.0) / (fpH10Cal - fpH7Cal)) * fpH10Cal;  // Calculate Y Intercept based off of pH Calibration slope.
      pH = ((10.0 - 7.0) / (fpH10Cal - fpH7Cal)) * rawAnalog + b;
    }

    if (rawAnalog >= fpH7Cal && rawAnalog <= fpH4Cal) {  // pH 7 - 4
      b = 7 - ((7.0 - 4.0) / (fpH7Cal - fpH4Cal)) * fpH7Cal;  // Calculate Y Intercept based off of pH Calibration slope.
      pH = ((7.0 - 4.0) / (fpH7Cal - fpH4Cal)) * rawAnalog + b;
    }

    if (rawAnalog >= fpH4Cal && rawAnalog <= fpH0Cal) {  // pH 4 - 0
      b = 4 - ((4.0 - 0.0) / (fpH4Cal - fpH0Cal)) * fpH4Cal;  // Calculate Y Intercept based off of pH Calibration slope.
      pH = ((4.0 - 0.0) / (fpH4Cal - fpH0Cal)) * rawAnalog + b;
    }
    fpHAnalogArray[j] = pH;
    j++;
  }
   
  if (lCurrentMillis - lLastSecond >= 1000) {  // Once 1000ms have passed...
    lLastSecond = lCurrentMillis;  // Save the current time.
    j = 0; // Resets j to 0.
    char maxIndex = 0; // Resets maxIndex to 0.
    char minIndex = 0; // Resets minIndex to 0.

    for (int i = 0; i < 10; i++) {  // Check each reading that we put in the array if our max or min number is higher or lower.
      if (fpHAnalogArray[i] > fpHAnalogArray[maxIndex]) {
        maxIndex = i;
      }
      if (fpHAnalogArray[i] < fpHAnalogArray[minIndex]) {
        minIndex = i;
      }
    }

    fpHAnalogArray[maxIndex] = 0; // Set the highest value in the pH array to 0 so it is not counted in average.
    fpHAnalogArray[minIndex] = 0; // Set the lowest value in the pH array to 0 so it is not counted in average.
    float fSum = 0;  // Initializes sum variable to 0 for use with determining average pH over the given 10 readings.
    unsigned char cCount = 0;  // Initializes count variable to 0, which holds how many pH readings above 0 we have. Using Char to save memory.

    for (int i = 0; i < 10; i++) {  // Check each value in pH array. If not 0, add it to sum and increase count.
      if (fpHAnalogArray[i] != 0) {
        fSum += fpHAnalogArray[i];
        cCount++;
      }
    }
    fpHAverage = fSum/cCount; // Calculate average of pH readings over 1 second
  }
}

void loop() {
  lCurrentMillis = millis();
  calculateAveragepH();

  if (fpHTarget != 0) // If we have a pH Target set....
  {
    if (!bWaitForMix) // If we are not waiting for mixture to become homogenous...
    {
      if (fpHAverage <= (fpHTarget - 0.2))  //If pH drifts from Target pH - 0.2...
      {
        bpHUnderRange = true;
        bpHOverRange = false;
      }
  
      else if (fpHAverage >= (fpHTarget + 0.2))  //If pH drifts from Target pH + 0.2...
      {
        bpHUnderRange = false;
        bpHOverRange = true;
      }
      
      else  // If pH is within range...
      {
        bpHUnderRange = false;
        bpHOverRange = false;
      }

      if (bpHUnderRange || bpHOverRange)  // If pH is out of range...
      {
        if (!bpHTimerStarted) // If we don't have a timer started already, then start a timer. If we do, then see if timer has expired.
        {
          lTimerStartMillis = lCurrentMillis;
          bpHTimerStarted = true;
        }
        else  // Timer already started
        {
          if (lCurrentMillis >= lTimerStartMillis + 5000) // Check if timer is expired...
          {
            // Timer expired. Do the things.
            bpHTimerEnded = true;
          }
        }
      }
      else  // If pH is no longer out of range...
      {
        bpHTimerStarted = false;
        bpHTimerEnded = false;
      }   
    }

    else  // If we are waiting for mixture to become homogenous
    {
    }
  }
  uiHandler();
}