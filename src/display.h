#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Define display parameters
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3c // Screen Address, either 0x3c or 0x3d

class UI {
    public:
        UI():display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1) {}
        void setupDisplay();
        void updateDisplay();
    private:
        Adafruit_SSD1306 display;
};
