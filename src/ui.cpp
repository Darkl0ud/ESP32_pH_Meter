#include <ui.h>

void UI::setupDisplay() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { 
    Serial.println(F("SSD1306 Allocation FAILED!"));
    for(;;); // Don't proceed, loop forever
  }
}

//void UI::drawCursor(){
//  if (optionSelected)
//  {
//    display.drawBitmap(32, menuOptionHighlighted * 16 + 1, rightArrowBitmap, 14, 14, 1);
//  }
//  else
//  {
//    display.drawBitmap(32, menuOptionHighlighted * 16 + 1, leftArrowBitmap, 14, 14, 1);
//  }
//}

void UI::drawMainMenu(int mainMenuSelection){
  int i = 0;
  display.setTextSize(1);
  display.setFont(NULL);

  while (i < 4)
  {
    if (i == mainMenuSelection)
    {
      // Make our selected menu item highlighted
      display.setTextColor(BLACK, WHITE);
      display.fillRoundRect(0, mainMenuSelection * 16, 33, 16, 7, 1);
      display.setCursor(menuTextCoords[i][0],menuTextCoords[i][1]);
      display.print(F(menuTextChars[i]));
    }
    else
    {
      display.setTextColor(WHITE);
      display.drawRoundRect(0, i * 16, 33, 16, 7, 1);
      display.setCursor(menuTextCoords[i][0],menuTextCoords[i][1]);
      display.print(F(menuTextChars[i]));
    }
   
    i++;
  }
}

void UI::drawpH(float pH)
{
  display.setTextColor(WHITE);
  display.setTextSize(3);
  display.setFont(&Picopixel);
  display.setCursor(38, 17);
  display.println("pH");
  display.setTextSize(4);
  display.setCursor(64, 20);
  display.println(pH);
}

void UI::drawAlarmMenu(float phAlarmTriggerVal){
  display.setTextColor(WHITE);
  display.setTextSize(3);
  display.setFont(&Picopixel);
  display.setCursor(38, 17);
  display.println("pH");
  display.setTextSize(4);
  display.setCursor(64, 20);
  display.println(phAlarmTriggerVal);
}

void UI::drawCalMenu(){
  
}

void UI::drawSysMenu(){
  
}

void UI::clearDisplay(){
  display.clearDisplay();
}

void UI::updateDisplay(){
  display.display();
}