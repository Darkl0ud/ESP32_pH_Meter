#include <Arduino.h>
#include <Wire.h>
#include <display.h>

UI ui;

// Defines I2C pins for SSD1306 screen
#define I2C_SCL_PIN 13
#define I2C_SDA_PIN 14

// Set to the corresponding pin for the pH sensor
#define PH_READ_PIN 8

// Set to the corresponding pins for the buttons, set into an array
#define UP_BUTTON_PIN 41
#define DOWN_BUTTON_PIN 38
#define LEFT_BUTTON_PIN 35
#define RIGHT_BUTTON_PIN 47
const int arrowPins[] = {UP_BUTTON_PIN, DOWN_BUTTON_PIN, LEFT_BUTTON_PIN, RIGHT_BUTTON_PIN};

// IRAM calls for button change state functions
void IRAM_ATTR upButtonChanged();
void IRAM_ATTR downButtonChanged();
void IRAM_ATTR leftButtonChanged();
void IRAM_ATTR rightButtonChanged();

// Put button interrupt functions into an array
void (*buttonInterruptHandlers[])() = {upButtonChanged, downButtonChanged, leftButtonChanged, rightButtonChanged};

// Bools store if we pressed the button when its corresponding IRAM function is called
bool upPressed = false;
bool downPressed= false;
bool leftPressed= false;
bool rightPressed= false;

// Time based variables for calculating pH average over time
unsigned long currentMillis = millis();
static unsigned long lastpHMillis = 0;
static unsigned long lastSecond = 0;
static unsigned long timerStartMillis = 0;

// Variables used or reading pH value and averaging
float rawAnalog = 0;
float pH = 0;
float pHAnalogArray[10];   // Array used for pH reading calculations.
float pHAverage = 0;
int j = 0;

// Variables used for pH calibration
float pH0Cal = 1;  // Default = 1, shouldn't really need to be changed.
float pH4Cal = 0.622;  // Voltage in pH 4 buffer, default = 0.7146.
float pH7Cal = 0.5126;  // Voltage in pH 7 buffer, default 0.5.
float pH10Cal = 0.2865; // Voltage in pH 10 buffer, default 0.2865.
float pH14Cal = 0;  // Default = 0, shouldn't really need to be changed.
