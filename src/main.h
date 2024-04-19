/* Made by Darkloud using DIY-MORE PH-4502C pH controller board and used specifically
 on the ESP32-S3 DevKitC */

#include <Arduino.h>

#define PH_READ_PIN 8 // GPIO 8, assigned to pH probe analog input.
#define UP_BUTTON_PIN 41
#define DOWN_BUTTON_PIN 38
#define LEFT_BUTTON_PIN 35
#define RIGHT_BUTTON_PIN 47

bool upPressed;
bool downPressed;
bool leftPressed;
bool rightPressed;
bool pHTimerStarted = false; // The millis() start of the pH timer. After timer has expired, this should become false and bTimerEnded true.
bool pHTimerEnded = false;
bool pHOverRange;
bool pHUnderRange;
int j;
unsigned long currentMillis = millis();
static unsigned long lastpHMillis;  // Last millisec trig.
static unsigned long lastSecond; // Last second trig.
static unsigned long timerStartMillis;
float rawAnalog;
float pHAnalogArray[10];   // Array used for pH reading calculations.
float pH0Cal = 1;  // Default = 1, shouldn't really need to be changed.
float pH4Cal = 0.622;  // Voltage in pH 4 buffer, default = 0.7146.
float pH7Cal = 0.5126;  // Voltage in pH 7 buffer, default 0.5.
float pH10Cal = 0.2865; // Voltage in pH 10 buffer, default 0.2865.
float pH14Cal = 0;  // Default = 0, shouldn't really need to be changed.
float pHAverage;
float pHTarget = 0;