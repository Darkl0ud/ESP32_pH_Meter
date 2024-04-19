#include <main.h>

void IRAM_ATTR buttonUpPressed() {
  if (digitalRead(UP_BUTTON_PIN) == 1)  {
    upPressed = true;
  }
  else  {
    upPressed = false;
  }
}

void IRAM_ATTR buttonDownPressed() {
  if (digitalRead(DOWN_BUTTON_PIN) == 1)  {
    downPressed = true;
  }
  else  {
    downPressed = false;
  }
}

void IRAM_ATTR buttonLeftPressed() {
  if (digitalRead(LEFT_BUTTON_PIN) == 1)  {
    leftPressed = true;
  }
  else  {
    leftPressed = false;
  }
}

void IRAM_ATTR buttonRightPressed() {
  if (digitalRead(RIGHT_BUTTON_PIN) == 1)  {
    rightPressed = true;
  }
  else  {
    rightPressed = false;
  }
}


void setup()  {
  Serial.begin(115200);
  pinMode(PH_READ_PIN, INPUT);
  pinMode(UP_BUTTON_PIN, INPUT_PULLDOWN);
  pinMode(DOWN_BUTTON_PIN, INPUT_PULLDOWN);
  pinMode(LEFT_BUTTON_PIN, INPUT_PULLDOWN);
  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLDOWN);
  attachInterrupt(UP_BUTTON_PIN, buttonUpPressed, CHANGE);
  attachInterrupt(DOWN_BUTTON_PIN, buttonDownPressed, CHANGE);
  attachInterrupt(LEFT_BUTTON_PIN, buttonLeftPressed, CHANGE);
  attachInterrupt(RIGHT_BUTTON_PIN, buttonRightPressed, CHANGE);
}

void calculateAveragepH() {
    if (currentMillis - lastpHMillis >= 100)  { // Once 100ms have passed...
    lastpHMillis = currentMillis;  // Save the current time.
    rawAnalog = analogRead(PH_READ_PIN) / 4095.0; // Read from pH probe. Gives an analog 0-1 value.
    float b;
    float pH;

    if (rawAnalog >= pH14Cal && rawAnalog <= pH10Cal) {  // pH 14 - 10
      b = 14 - ((14.0 - 10.0) / (pH14Cal - pH10Cal)) * pH14Cal;  // Calculate Y Intercept based off of pH Calibration slope.
      pH = ((14.0 - 10.0) / (pH14Cal - pH10Cal)) * rawAnalog + b;
    }
    
    if (rawAnalog >= pH10Cal && rawAnalog <= pH7Cal)  {  // pH 10 - 7
      b = 10 - ((10.0 - 7.0) / (pH10Cal - pH7Cal)) * pH10Cal;  // Calculate Y Intercept based off of pH Calibration slope.
      pH = ((10.0 - 7.0) / (pH10Cal - pH7Cal)) * rawAnalog + b;
    }

    if (rawAnalog >= pH7Cal && rawAnalog <= pH4Cal) {  // pH 7 - 4
      b = 7 - ((7.0 - 4.0) / (pH7Cal - pH4Cal)) * pH7Cal;  // Calculate Y Intercept based off of pH Calibration slope.
      pH = ((7.0 - 4.0) / (pH7Cal - pH4Cal)) * rawAnalog + b;
    }

    if (rawAnalog >= pH4Cal && rawAnalog <= pH0Cal) {  // pH 4 - 0
      b = 4 - ((4.0 - 0.0) / (pH4Cal - pH0Cal)) * pH4Cal;  // Calculate Y Intercept based off of pH Calibration slope.
      pH = ((4.0 - 0.0) / (pH4Cal - pH0Cal)) * rawAnalog + b;
    }
    pHAnalogArray[j] = pH;
    j++;
  }
   
  if (currentMillis - lastSecond >= 1000) {  // Once 1000ms have passed...
    lastSecond = currentMillis;  // Save the current time.
    j = 0; // Resets j to 0.
    char maxIndex = 0; // Resets maxIndex to 0.
    char minIndex = 0; // Resets minIndex to 0.

    for (int i = 0; i < 10; i++) {  // Check each reading that we put in the array if our max or min number is higher or lower.
      if (pHAnalogArray[i] > pHAnalogArray[maxIndex]) {
        maxIndex = i;
      }
      if (pHAnalogArray[i] < pHAnalogArray[minIndex]) {
        minIndex = i;
      }
    }

    pHAnalogArray[maxIndex] = 0; // Set the highest value in the pH array to 0 so it is not counted in average.
    pHAnalogArray[minIndex] = 0; // Set the lowest value in the pH array to 0 so it is not counted in average.
    float Sum = 0;  // Initializes sum variable to 0 for use with determining average pH over the given 10 readings.
    unsigned char countsOverZero = 0;  // Initializes count variable to 0, which holds how many pH readings above 0 we have. Using Char to save memory.

    for (int i = 0; i < 10; i++) {  // Check each value in pH array. If not 0, add it to sum and increase count.
      if (pHAnalogArray[i] != 0) {
        Sum += pHAnalogArray[i];
        countsOverZero++;
      }
    }
    pHAverage = Sum/countsOverZero; // Calculate average of pH readings over 1 second
    Serial.println(pHAverage);
  }
}

void loop() {
  currentMillis = millis();
  calculateAveragepH();
  
  if (pHTarget != 0) // If we have a pH Target set....
  {
    
    if (pHAverage <= (pHTarget - 0.2))  //If pH drifts from Target pH - 0.2...
    {
      pHUnderRange = true;
      pHOverRange = false;
    }
    else if (pHAverage >= (pHTarget + 0.2))  //If pH drifts from Target pH + 0.2...
    {
      pHUnderRange = false;
      pHOverRange = true;
    }
    else  // If pH is within range...
    {
      pHUnderRange = false;
      pHOverRange = false;
    }
  }
}