/* Made by Darkloud for the DIY-MORE PH-4502C pH controller board and used specifically
 on the ESP32-S3 DevKitC */

#include <Arduino.h>

#define RGB_BRIGHTNESS 1 //  Brightness setting for RGB LEDs
#define PH_READ_PIN 8 // GPIO 8, assigned to pH probe analog input

float fpHAnalogArray[10];
int j;
unsigned long lCurrentMillis = millis();
static unsigned long lLastMillis;  // Last millisec trig
static unsigned long lLastSecond; // Last second trig
float rawAnalog;
float fpH0Cal = 1;  // Default = 1, shouldn't really need to be changed
float fpH4Cal = 0.61440;  // Voltage in pH 4 buffer, default = 0.7146
float fpH7Cal = 0.502;  // Voltage in pH 7 buffer, default 0.5
float fpH10Cal = 0.2865; // Voltage in pH 10 buffer, default 0.2865
float fpH14Cal = 0;  // Default = 0, shouldn't really need to be changed