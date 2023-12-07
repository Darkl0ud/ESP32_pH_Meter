/* Made by Darkloud using DIY-MORE PH-4502C pH controller board and used specifically
 on the ESP32-S3 DevKitC */
 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

class DisplayUI{
    public:
        void DisplayUpdate(char cSelectedMenuTab, float fpHAverage, float fpHTarget, bool bSettingsMenuActive, bool bpHTimerEnded); // Clears and draws the screen with current info.
        void InitDisplay();

    private:
        void DrawMenuTabCursor(char cSelectedMenuTab);  // Tab to draw the menu cursor on. 0 = pH, 1 = EC, 2 = Settings
        void DrawArrow(int iArrowX, int iArrowY, char cDirection);
        void DrawMenuTabs();

};