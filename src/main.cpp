/* Made by Darkloud for the DIY-MORE PH-4502C pH controller board and used specifically
 on the ESP32-S3 DevKitC */

#include <Arduino.h>

#define RGB_BRIGHTNESS 1 //  Brightness setting for RGB LEDs
#define PH_READ_PIN 8 // GPIO 8, assigned to pH probe analog input

float fpHArray[10];
int j;
unsigned long currentMillis = millis();
static unsigned long fLastMillis;  // Last millisec trig
static unsigned long fLastSecond; // Last second trig

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
  currentMillis = millis();

  if (currentMillis - fLastMillis >= 100) {
    fLastMillis = currentMillis;  // Save the current time
    fpHArray[j] = analogRead(PH_READ_PIN) / 4095.0 * 3.3 * 4.24; // 4.24 = 14/3.3
    j++;
  }
   
  if (currentMillis - fLastSecond >= 1000) {
    fLastSecond = currentMillis;  // Save the current time
    j = 0; // Resets j to 0
    int maxIndex = 0; // Resets maxIndex to 0
    int minIndex = 0; // Resets minIndex to 0

    for (int i = 0; i < 10; i++) {  // Check each reading that we put in the array if our max or min number is higher or lower
      if (fpHArray[i] > fpHArray[maxIndex]) {
        maxIndex = i;
      }
      if (fpHArray[i] < fpHArray[minIndex]) {
        minIndex = i;
      }
    }

    fpHArray[maxIndex] = 0; // Set the highest value in the pH array to 0 so it is not counted in average.
    fpHArray[minIndex] = 0; // Set the lowest value in the pH array to 0 so it is not counted in average.
    float sum = 0;  // Initializes sum variable to 0 for use with determining average pH over the given 10 readings
    int count = 0;  // Initializes count variable to 0, which holds how many pH readings above 0 we have

    for (int i = 0; i < 10; i++) {  // Check each value in pH array. If not 0, add it to sum and increase count.
      if (fpHArray[i] != 0) {
        sum += fpHArray[i];
        count++;
      }
    }
    
    float fpHAverage = sum/count; // Calculate average of pH readings over 1 second
  }  
}

void loop() {
  SetDeviceState(0);
  RunpH();
}