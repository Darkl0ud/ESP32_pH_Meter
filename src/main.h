/* Made by Darkloud using DIY-MORE PH-4502C pH controller board and used specifically
 on the ESP32-S3 DevKitC */

#include <Arduino.h>
#include <displayui.h>

#define RGB_BRIGHTNESS 1 //  Brightness setting for RGB LEDs.
#define PH_READ_PIN 8 // GPIO 8, assigned to pH probe analog input.
#define UP_BUTTON_PIN 41
#define DOWN_BUTTON_PIN 38
#define LEFT_BUTTON_PIN 35
#define RIGHT_BUTTON_PIN 47
#define I2C_SCL_PIN 14
#define I2C_SDA_PIN 13

DisplayUI dUI;
char cDevState; // For future use. There is a function called SetDeviceState that should be used with this variable.
char cSelectedMenuTab;  // UI Variable for selected menu tab on the left of the display. 0 = pH, 1 = EC, 2 = Settings.
char cpHPumpMode;   // Current pH Up/Down pump mode. 0 = Off, 1 = Up, 2 = Down
bool bMenuTabActive;    // UI Variable for if the current cSelectedMenuTab is actively being modified by user.
bool bUpPressed;
bool bDownPressed;
bool bLeftPressed;
bool bRightPressed;
bool bpHTimerStarted = false; // The millis() start of the pH timer. After timer has expired, this should become false and bTimerEnded true.
bool bpHTimerEnded = false;
bool bpHOverRange;
bool bpHUnderRange;
bool bWaitForMix;   // Set true when we are waiting for mixture to become homogenous based on a timer.
int j;
unsigned long lCurrentMillis = millis();
static unsigned long lLastpHMillis;  // Last millisec trig.
static unsigned long lLastSecond; // Last second trig.
static unsigned long lTimerStartMillis;
float rawAnalog;
float fpHAnalogArray[10];   // Array used for pH reading calculations.
float fpH0Cal = 1;  // Default = 1, shouldn't really need to be changed.
float fpH4Cal = 0.622;  // Voltage in pH 4 buffer, default = 0.7146.
float fpH7Cal = 0.5126;  // Voltage in pH 7 buffer, default 0.5.
float fpH10Cal = 0.2865; // Voltage in pH 10 buffer, default 0.2865.
float fpH14Cal = 0;  // Default = 0, shouldn't really need to be changed.
float fpHAverage;
float fpHTarget = 0;