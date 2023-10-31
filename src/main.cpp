#include <Arduino.h>

#define RGB_BRIGHTNESS 10 //  Brightness setting for RGB LEDs
#define PH_READ_PIN 8 // GPIO 8, assigned to pH probe analog input
//#define EC_READ_PIN 37 // GPIO 37, assigned to EC probe analog input
//#define PH_UP_PUMP 36 // GPIO 36, assigned to activating pH Up pump
//#define PH_DOWN_PUMP 35 // GPIO 35, assigned to activating pH Down pump
//#define NUTE_A_PUMP 34 // GPIO 34, assigned to activating Nutrient Part A pump
//#define NUTE_B_PUMP 33 // GPIO 33, assigned to activating Nutrient Part B pump
//#define NUTE_EPSOM_PUMP 32 // GPIO 32, assigned to activating Epsom Salt pump

unsigned long previousMillis = 0;  // will store last time the task was executed

void setup() {
  Serial.begin(115200);
  neopixelWrite(RGB_BUILTIN,RGB_BRIGHTNESS,0,0);

  pinMode(PH_READ_PIN, INPUT);
  //pinMode(EC_READ_PIN, INPUT);
  //pinMode(PH_UP_PUMP, OUTPUT);
  //pinMode(PH_DOWN_PUMP, OUTPUT);
  //pinMode(NUTE_A_PUMP, OUTPUT);
  //pinMode(NUTE_B_PUMP, OUTPUT);
  //pinMode(NUTE_EPSOM_PUMP, OUTPUT);
}

void SetDeviceState(int state) {

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

void loop() {
  SetDeviceState(0);

  //float fAnalog_value = analogRead(PH_READ_PIN); 
  //float fVoltage = fAnalog_value / 4095 * 3.3;
  float fpH = analogRead(PH_READ_PIN) / 4095 * 3.3 * 4.24; // 4.24 = 14/3.3
  //Serial.print("fAnalog_value:"); 
  //Serial.print(fAnalog_value); 
  //Serial.print(" fVoltage:"); 
  //Serial.print(fVoltage); 
  //Serial.print("pH: "); 
  //Serial.println(fpH); ;
  
  unsigned long currentMillis = millis();  // grab the current time

   // Check if 1 second has elapsed
  if (currentMillis - previousMillis >= 1000) {
    // Save the current time
    previousMillis = currentMillis;

    // Perform your task here, this will repeat every 1 second
    // Code for the task goes here

    // Example: print a message every second
    Serial.println("Task executed every 1 second");
  }

}