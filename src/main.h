/* Made by Darkloud for the DIY-MORE PH-4502C pH controller board and used specifically
 on the ESP32-S3 DevKitC */

#include <Arduino.h>

#define RGB_BRIGHTNESS 1 //  Brightness setting for RGB LEDs.
#define PH_READ_PIN 8 // GPIO 8, assigned to pH probe analog input.
#define UP_BUTTON_PIN 41
#define DOWN_BUTTON_PIN 38
#define LEFT_BUTTON_PIN 35
#define RIGHT_BUTTON_PIN 47

char cDevState; // For future use. There is a function called SetDeviceState that should be used with this variable.
char cCurrentMenu = 0;  // Holds value of the current menu viewed on screen. 0 = Home, 1 = pH, 2 = EC, 3 = Settings.
char cCurrentMenuTab = 0;   // The currently selected manu tab. 0 = Home, 1 = pH, 2 = EC, 3 = Settings.
char cCursorMode;   // 0 if cursor is selecting a menu tab, 1 if selecting a menu option.
bool bUpPressed = false;
bool bDownPressed = false;
bool bLeftPressed = false;
bool bRightPressed = false;
int j;
unsigned long lCurrentMillis = millis();
static unsigned long lLastMillis;  // Last millisec trig.
static unsigned long lLastSecond; // Last second trig.
float rawAnalog;
float fpHAnalogArray[10];   // Array used for pH reading calculations.
float fpH0Cal = 1;  // Default = 1, shouldn't really need to be changed.
float fpH4Cal = 0.622;  // Voltage in pH 4 buffer, default = 0.7146.
float fpH7Cal = 0.5126;  // Voltage in pH 7 buffer, default 0.5.
float fpH10Cal = 0.2865; // Voltage in pH 10 buffer, default 0.2865.
float fpH14Cal = 0;  // Default = 0, shouldn't really need to be changed.
float fpHAverage;
float fpHTarget;
