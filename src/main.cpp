#include <main.h>

// Button interrupt functions
void buttonUpPressed() {
  if (digitalRead(UP_BUTTON_PIN) == 1)  {
    upPressed = true;
  }
   
  else  {
    upPressed = false;
  }
}

void buttonDownPressed() {
  if (digitalRead(DOWN_BUTTON_PIN) == 1)  {
    downPressed = true;
  }

  else  {
    downPressed = false;
  }
}

void buttonLeftPressed() {
  if (digitalRead(LEFT_BUTTON_PIN) == 1)  {
    leftPressed = true;
  }

  else  {
    leftPressed = false;
  }
}

void buttonRightPressed() {
  if (digitalRead(RIGHT_BUTTON_PIN) == 1)  {
    rightPressed = true;
  }

  else  {
    rightPressed = false;
  }
}

void setupPins(){
  // Set pH probe
  pinMode(PH_READ_PIN, INPUT);
  
  // Set arrow keys
  for (int i = 0; i < sizeof(arrowPins) / sizeof(arrowPins[0]); i++) {
    pinMode(arrowPins[i], INPUT_PULLDOWN);
    attachInterrupt(arrowPins[i], buttonInterruptHandlers[i], CHANGE);
  }
}

// Returns calibrated pH value from input
float calculateCalibratedpH(float input, float pHRangeHigh, float pHRangeLow, 
                            float pHHighCal, float pHLowCal){
  float slope = (pHRangeHigh - pHRangeLow) / (pHHighCal - pHLowCal);
  float intercept = pHRangeHigh - slope * pHHighCal; 
  return slope * rawAnalog + intercept;
}

// Returns calibrated pH value depending on pH range
float getCalibratedpH(){
  rawAnalog = analogRead(PH_READ_PIN) / 4095.0; 

  if (rawAnalog >= pH14Cal && rawAnalog <= pH10Cal) {
    pH = calculateCalibratedpH(rawAnalog, 14, 10, pH14Cal, pH10Cal);
  }

  if (rawAnalog >= pH10Cal && rawAnalog <= pH7Cal)  {
    pH = calculateCalibratedpH(rawAnalog, 10, 7, pH10Cal, pH7Cal);
  }

  if (rawAnalog >= pH7Cal && rawAnalog <= pH4Cal) {
    pH = calculateCalibratedpH(rawAnalog, 7, 4, pH7Cal, pH4Cal);
  }

  if (rawAnalog >= pH4Cal && rawAnalog <= pH0Cal) {  
    pH = calculateCalibratedpH(rawAnalog, 4, 0, pH4Cal, pH0Cal);
  }
  
  return pH;
}

void calculateAveragepH() {
  // Once 100ms have passed...
  if (currentMillis - lastpHMillis >= 100)  { 
    // Save the current time.
    lastpHMillis = currentMillis;  



    pHAnalogArray[j] = getCalibratedpH();  
    j++;
  }
   
  // Once 1000ms have passed...
  if (currentMillis - lastSecond >= 1000) {  
    lastSecond = currentMillis;
    j = 0;
    char maxIndex = 0;
    char minIndex = 0;

    // Check each reading that we put in the array if our max or min number is higher or lower.
    for (int i = 0; i < 10; i++) {  
      if (pHAnalogArray[i] > pHAnalogArray[maxIndex]) {
        maxIndex = i;
      }

      if (pHAnalogArray[i] < pHAnalogArray[minIndex]) {
        minIndex = i;
      }
    }

    // Set the highest value in the pH array to 0 so it is not counted in average.
    pHAnalogArray[maxIndex] = 0; 
    // Set the lowest value in the pH array to 0 so it is not counted in average.
    pHAnalogArray[minIndex] = 0; 

    // For use with determining average pH over the given 10 readings.
    float Sum = 0;  
    // Holds how many pH readings above 0 we have.
    unsigned char countsOverZero = 0;  
    
    // Check each value in pH array. If not 0, add it to sum and increase count.
    for (int i = 0; i < 10; i++) {  
      if (pHAnalogArray[i] != 0) {
        Sum += pHAnalogArray[i];
        countsOverZero++;
      }
    }

    // Calculate average of pH readings over 1 second
    pHAverage = Sum/countsOverZero; 
    Serial.println(pHAverage);
  }
}
 
void setup()  {
  Serial.begin(115200);
  setupPins();
}

void loop() {
  currentMillis = millis();
  calculateAveragepH();
}