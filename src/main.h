#include <sm.h>

// Defines I2C pins for SSD1306 screen
#define I2C_SCL_PIN 13
#define I2C_SDA_PIN 14

// Set to corresponding pin for alarm high, this pin goes high if alarm state is met
#define ALARM_HIGH_PIN 16

// Set to the corresponding pins for the buttons, set into an array
#define UP_BUTTON_PIN 4
#define DOWN_BUTTON_PIN 5
#define LEFT_BUTTON_PIN 6
#define RIGHT_BUTTON_PIN 7
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
bool leftPressed = false;
bool rightPressed= false;
unsigned long lastInputTime;