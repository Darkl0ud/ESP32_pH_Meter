#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/Picopixel.h>


//Define display parameters
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3c // Screen Address, either 0x3c or 0x3d

const uint8_t leftArrowBitmap[] = {0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x07, 0x00, 
                                   0x0e, 0x00, 0x1c, 0x00, 0x3f, 0xf0, 0x3f, 0xf0,
                                   0x1c, 0x00, 0x0e, 0x00, 0x07, 0x00, 0x02, 0x00, 
                                   0x00, 0x00, 0x00, 0x00};

const uint8_t rightArrowBitmap[] = {0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x03, 0x00, 
                                    0x01, 0x80, 0x3f, 0xc0, 0x3f, 0xc0, 0x01, 0x80, 
                                    0x03, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00};
    
// Char array for menu text
const char menuTextChars[4][6] = {
    "Info",
    "Alarm",
    "Cal.",
    "Sys."
};

// Coordinates for menu text
const int menuTextCoords[4][2] = {
    {4, 4},
    {2, 20},
    {6, 36},
    {6, 52}
};

class UI {

    public:
        UI():display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1) {}
        
        void setupDisplay();
        void clearDisplay();
        void updateDisplay();
        void drawpH(float pH);
        void drawMainMenu(int mainMenuSelection);
        void drawAlarmMenu(float phAlarmTriggerVal);
        void drawCalMenu();
        void drawSysMenu();
        //void drawCursor();

    private:
        Adafruit_SSD1306 display;
};