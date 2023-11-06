/* Made by Darkloud for the DIY-MORE PH-4502C pH controller board and used specifically
 on the ESP32-S3 DevKitC */

#include <main.h>

void setup() {
  neopixelWrite(RGB_BUILTIN,RGB_BRIGHTNESS,0,0);
  Serial.begin(115200);
  pinMode(PH_READ_PIN, INPUT);
}

void SetDeviceState(int state) {  // Sets various indicators depending upon system status

  switch (state) {

  case 0: // Normal State
    delay(1); // Dalay to allow LED to change. Not sure why this is needed...?
    neopixelWrite(RGB_BUILTIN,0,RGB_BRIGHTNESS,0);  // Set Status RGB Green
    break;

  case 1: // Fault State
    delay(1); // Dalay to allow LED to change. Not sure why this is needed...?
    neopixelWrite(RGB_BUILTIN,RGB_BRIGHTNESS,0,0);  // Set Status RGB Red
    break;
  
  case 2: // Notification State
    delay(1); // Dalay to allow LED to change. Not sure why this is needed...?
    neopixelWrite(RGB_BUILTIN,0,0,RGB_BRIGHTNESS);  // Set Status RGB Blue
    break;

  default:
    break;
  }
}

void RunpH() {
  lCurrentMillis = millis();

  if (lCurrentMillis - lLastMillis >= 100) { // Once 100ms have passed...
    lLastMillis = lCurrentMillis;  // Save the current time

    rawAnalog = analogRead(PH_READ_PIN) / 4095.0; // Read from pH probe. Gives an analog 0-1 value.
    float b;
    float pH;

    if (rawAnalog >= fpH14Cal && rawAnalog <= fpH10Cal)  // pH 14 - 10
    {
      b = 14 - ((14.0 - 10.0) / (fpH14Cal - fpH10Cal)) * fpH14Cal;  // Calculate Y Intercept based off of pH Calibration slope
      pH = ((14.0 - 10.0) / (fpH14Cal - fpH10Cal)) * rawAnalog + b;
    }
    
    if (rawAnalog >= fpH10Cal && rawAnalog <= fpH7Cal)  // pH 10 - 7
    {
      b = 10 - ((10.0 - 7.0) / (fpH10Cal - fpH7Cal)) * fpH10Cal;  // Calculate Y Intercept based off of pH Calibration slope
      pH = ((10.0 - 7.0) / (fpH10Cal - fpH7Cal)) * rawAnalog + b;
    }

    if (rawAnalog >= fpH7Cal && rawAnalog <= fpH4Cal)  // pH 7 - 4
    {
      b = 7 - ((7.0 - 4.0) / (fpH7Cal - fpH4Cal)) * fpH7Cal;  // Calculate Y Intercept based off of pH Calibration slope
      pH = ((7.0 - 4.0) / (fpH7Cal - fpH4Cal)) * rawAnalog + b;
    }

    if (rawAnalog >= fpH4Cal && rawAnalog <= fpH0Cal)  // pH 4 - 0
    {
      b = 4 - ((4.0 - 0.0) / (fpH4Cal - fpH0Cal)) * fpH4Cal;  // Calculate Y Intercept based off of pH Calibration slope
      pH = ((4.0 - 0.0) / (fpH4Cal - fpH0Cal)) * rawAnalog + b;
    }

    fpHAnalogArray[j] = pH;
    j++;
  }
   
  if (lCurrentMillis - lLastSecond >= 1000) {  // Once 1000ms have passed...
    lLastSecond = lCurrentMillis;  // Save the current time
    j = 0; // Resets j to 0
    char maxIndex = 0; // Resets maxIndex to 0
    char minIndex = 0; // Resets minIndex to 0

    for (int i = 0; i < 10; i++) {  // Check each reading that we put in the array if our max or min number is higher or lower
      if (fpHAnalogArray[i] > fpHAnalogArray[maxIndex]) {
        maxIndex = i;
      }
      if (fpHAnalogArray[i] < fpHAnalogArray[minIndex]) {
        minIndex = i;
      }
    }

    fpHAnalogArray[maxIndex] = 0; // Set the highest value in the pH array to 0 so it is not counted in average.
    fpHAnalogArray[minIndex] = 0; // Set the lowest value in the pH array to 0 so it is not counted in average.
    float fSum = 0;  // Initializes sum variable to 0 for use with determining average pH over the given 10 readings
    unsigned char cCount = 0;  // Initializes count variable to 0, which holds how many pH readings above 0 we have. Char to save memory

    for (int i = 0; i < 10; i++) {  // Check each value in pH array. If not 0, add it to sum and increase count.
      if (fpHAnalogArray[i] != 0) {
        fSum += fpHAnalogArray[i];
        cCount++;
      }
    }
    float fpHAverage = fSum/cCount; // Calculate average of pH readings over 1 second
    Serial.println(fpHAverage);
  }  
}

void CalibratepH() {
  Serial.println("Please rinse off probe and place in pH 4 solution, wait until reading is stable, then press button to lock in calibration. ");
  // Wait for button press.
  rawAnalog = analogRead(PH_READ_PIN) / 4095.0;
  fpH4Cal = rawAnalog;

    Serial.println("Please rinse off probe and place in pH 7 solution, wait until reading is stable, then press button to lock in calibration. ");
  // Wait for button press.
  rawAnalog = analogRead(PH_READ_PIN) / 4095.0;
  fpH7Cal = rawAnalog;

    Serial.println("Please rinse off probe and place in pH 10 solution, wait until reading is stable, then press button to lock in calibration. ");
  // Wait for button press.
  rawAnalog = analogRead(PH_READ_PIN) / 4095.0;
  fpH10Cal = rawAnalog;
}

void loop() {
  SetDeviceState(0);
  RunpH();
}