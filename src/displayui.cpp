/* Made by Darkloud using DIY-MORE PH-4502C pH controller board and used specifically
 on the ESP32-S3 DevKitC */

#include <displayui.h>

Adafruit_SSD1306 dd(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);    // Direct calls to SSD1306 libraries to write to display.

void DisplayUI::InitDisplay()  {    // Initializes display, sets to 'dd' (Display Direct) TD: POSSIBLY PUT THIS IN CONSTRUCTOR??
    if (!dd.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { // Initialize SD1306 Display
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
      ;
  }
}

void DisplayUI::DrawArrow(int iArrowX, int iArrowY, char cDirection)  { // Draws an arrow at the specified point, in the specified direction. (X, Y, U/D/L/R)
  switch (cDirection)   {
  case 'U':
  case 'u':
    dd.fillTriangle(
    iArrowX + 6, iArrowY, 
    iArrowX, iArrowY + 6, 
    iArrowX, iArrowY - 6, 
    SSD1306_WHITE);
    break;
  
  case 'D':
  case 'd':
    dd.fillTriangle(
    iArrowX + 6, iArrowY, 
    iArrowX, iArrowY + 6, 
    iArrowX, iArrowY - 6, 
    SSD1306_WHITE);
    break;
  
  case 'L':
  case 'l':
    dd.fillTriangle(
    iArrowX + 6, iArrowY, 
    iArrowX, iArrowY + 6, 
    iArrowX, iArrowY - 6, 
    SSD1306_WHITE);
    break;
  
  case 'R':
  case 'r':
    dd.fillTriangle(
    iArrowX + 6, iArrowY, 
    iArrowX, iArrowY + 6, 
    iArrowX, iArrowY - 6, 
    SSD1306_WHITE);
    break;
 
  default:
    break;
  }
}

void DisplayUI::DrawMenuTabs(){ // Draws the menu tabs on the left side of screen

    dd.setTextSize(2);
    dd.setTextColor(SSD1306_WHITE);
    dd.setCursor(17, 0);
    dd.println(F("pH"));
    dd.setCursor(17, 26);
    dd.println(F("EC"));
    dd.setCursor(10, 50);
    dd.println(F("Set"));
}

void DisplayUI::DrawMenuTabCursor(char cSelectedMenuTab) {  // Tab to draw the menu cursor on. 0 = pH, 1 = EC, 2 = Settings 
    switch (cSelectedMenuTab)   {
    case 0:
        DrawArrow(0,6,'U');
        break;
    
    case 1:
        DrawArrow(0,32,'U');
        break;
    
    case 2:
        DrawArrow(0,57,'U');
        break;
    
    default:
        break;
    }
}

void DisplayUI::DisplayUpdate(char cSelectedMenuTab, float fpHAverage, // Clears and draws the screen with current info.
                            float fpHTarget, bool bSettingsMenuActive, 
                            bool bpHTimerEnded)   
{   
    
    dd.clearDisplay();
    DrawMenuTabs();

    switch (cSelectedMenuTab)   {
    case 0:
        DrawMenuTabCursor(cSelectedMenuTab);
        dd.setCursor(56, 20);  // Set cursor to right side of display
        dd.setTextSize(3);
        char charBuffer[5];  // Buffer for the formatted pH value
        dtostrf(fpHAverage, 4, 1, charBuffer);
        dd.println(charBuffer);
        break;
    
    case 1:
        DrawMenuTabCursor(cSelectedMenuTab);
        dd.setCursor(56, 20);  // Set cursor to right side of display
        dd.setTextSize(3);
        dd.println(F("NAN"));
        break;
    
    case 2:

        switch (bSettingsMenuActive)    {
        case 0:
            DrawMenuTabCursor(cSelectedMenuTab);
            dd.setCursor(56, 20);  // Set cursor to right side of display
            dd.setTextSize(3);
            char charBuffer[5];  // Buffer for the formatted pH value
            dtostrf(fpHTarget, 4, 1, charBuffer);
            dd.println(charBuffer);
            break;
        
        case 1:
            dd.setCursor(56, 20);  // Set cursor to right side of display
            dd.setTextSize(3);
            dtostrf(fpHTarget, 4, 1, charBuffer);
            dd.println(charBuffer);
            DrawArrow(98,6,'R');
            DrawArrow(98,57,'R');
            break;

        default:
            break;
        }
        break;
    default:
        break;
    }
  Serial.println(bpHTimerEnded);

    if (bpHTimerEnded == true)
    {
        DrawArrow(0,0,'U');
    }
    
    dd.display();
}
