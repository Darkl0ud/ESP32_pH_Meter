#include <main.h>

// Button interrupt functions
void upButtonChanged() {
  upPressed = true;
}

void downButtonChanged() {
  downPressed = true;
}

void leftButtonChanged() {
  leftPressed = true;
}

void rightButtonChanged() {
  rightPressed = true;
}

void setupPins(){
  // Set pH probe
  pinMode(PH_READ_PIN, INPUT);
  
  // Set arrow keys
  for (int i = 0; i < sizeof(arrowPins) / sizeof(arrowPins[0]); i++) {
    pinMode(arrowPins[i], INPUT_PULLDOWN);
    attachInterrupt(arrowPins[i], buttonInterruptHandlers[i], HIGH);
  }
}
 
void setup() {
  Serial.begin(115200);
  setupPins();
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN); // Initialize Wire with custom I2C SDA and SCL pins
  ui.setupDisplay();
}

void loop() {
  ui.clearDisplay();
  switch (mainMenuSelection)
  {
    case LeftMenuState::INFO:
      ui.drawpH(ph.getAveragepH(PH_READ_PIN));

      if (upPressed)    
      {   
        upPressed = false;    
        mainMenuSelection = LeftMenuState::SYS;
      }   
  
      if (downPressed)    
      {
        downPressed = false;    
        mainMenuSelection = LeftMenuState::ALARM;   
      }   
  
      if (leftPressed)    
      {   
        leftPressed = false;    
      }   
  
      if (rightPressed)   
      {   
        rightPressed = false;   
      }
    break;    
  
    case LeftMenuState::ALARM:

      if (!menuActive)
      {
        ui.drawAlarmMenu(pHAlarmTriggerVal);

        if (upPressed)    
        {   
          upPressed = false;    
          mainMenuSelection = LeftMenuState::INFO;    
        }   

        if (downPressed)    
        {   
          downPressed = false;    
          mainMenuSelection = LeftMenuState::CAL;   
        }   

        if (leftPressed)    
        {   
          leftPressed = false;    
        }   

        if (rightPressed)   
        {   
          rightPressed = false;   
          menuActive = true;    
        }         
      }
      //  Menu is active
      else
      {
        if (upPressed)    
        { 
          pHAlarmTriggerVal += 0.05;
          upPressed = false;    
        }   

        if (downPressed)    
        {
          pHAlarmTriggerVal -= 0.05;
          downPressed = false;    
        }   

        if (leftPressed)    
        {   
          leftPressed = false;
          menuActive = false;     
        }   

        if (rightPressed)   
        {   
          rightPressed = false;   
        }

        //Flash currently set pH alarm value
        if (millis() > flashTimer + 200)
        {
          ui.drawAlarmMenu(pHAlarmTriggerVal);

          if (millis() > flashTimer + 400)
          {
            flashTimer = millis();
          }
        }
      }
      
    break;    
  
    case LeftMenuState::CAL:    
      if (upPressed)    
      {   
        upPressed = false;    
        mainMenuSelection = LeftMenuState::ALARM;   
      }   
  
      if (downPressed)    
      {   
        downPressed = false;    
        mainMenuSelection = LeftMenuState::SYS;   
      }   
  
      if (leftPressed)    
      {   
        leftPressed = false;    
      }   
  
      if (rightPressed)   
      {   
        rightPressed = false;   
      }   
    break;    
  
    case LeftMenuState::SYS:    
      if (upPressed)    
      {   
        upPressed = false;    
        mainMenuSelection = LeftMenuState::CAL;   
      }   
  
      if (downPressed)    
      {   
        downPressed = false;    
        mainMenuSelection = LeftMenuState::INFO;    
      }   
  
      if (leftPressed)    
      {   
        leftPressed = false;    
      }   
  
      if (rightPressed)   
      {   
        rightPressed = false;   
      }   
    break;    
  
    default:    
      break;    
  }
  ui.drawMainMenu(int(mainMenuSelection));
 ui.updateDisplay(); 
}
