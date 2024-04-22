#include <display.h>

void UI::setupDisplay() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { 
    Serial.println(F("SSD1306 Allocation FAILED!"));
    for(;;); // Don't proceed, loop forever
  }
  else
  {
    display.setFont(NULL);
    display.setTextColor(SSD1306_WHITE);
  }
}

void UI::drawCursor(){
  if (optionSelected)
  {
    display.drawBitmap(32, menuOption * 16 + 1, rightArrowBitmap, 14, 14, 1);
  }
  else
  {
    display.drawBitmap(32, menuOption * 16 + 1, leftArrowBitmap, 14, 14, 1);
  }
}

void UI::drawpH(float pHin)
{
  display.setTextSize(4);
  display.setCursor(50, 20);
  display.setFont(&Picopixel);
  display.println(pHin);
  display.display();
}

void UI::drawMenuOptions()
{
  display.drawRoundRect(0, 0, 33, 16, 7, 1);
  display.drawRoundRect(0, 16, 33, 16, 7, 1);
  display.drawRoundRect(0, 32, 33, 16, 7, 1);
  display.drawRoundRect(0, 48, 33, 16, 7, 1);

  display.setFont(NULL);
  display.setTextSize(1);
  display.setCursor(4, 4);
  display.print(F("Info"));
  display.setCursor(2, 20);
  display.print(F("Alarm"));
  display.setCursor(6, 36);
  display.print(F("Cal."));
  display.setCursor(6, 52);
  display.print(F("Sys."));
}

void UI::updateDisplay(float pHin){
    display.clearDisplay();
    drawMenuOptions();
    drawCursor();
    drawpH(pHin);
}