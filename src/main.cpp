#include <main.h>

// Button interrupt functions
void upButtonChanged() {
  if (millis() >= lastInputTime + 100 && digitalRead(UP_BUTTON_PIN))
  {upPressed = true;}
  else
  {upPressed = false;}
}

void downButtonChanged() {
  if (millis() >= lastInputTime + 100 && digitalRead(DOWN_BUTTON_PIN))
  {downPressed = true;}
  else
  {downPressed = false;}
}

void leftButtonChanged() {
  if (millis() >= lastInputTime + 100 && digitalRead(LEFT_BUTTON_PIN))
  {leftPressed = true;}
  else
  {leftPressed = false;}
}

void rightButtonChanged() {
  if (millis() >= lastInputTime + 100 && digitalRead(RIGHT_BUTTON_PIN))
  {rightPressed = true;}
  else
  {rightPressed = false;}
}

void setupPins(){
  // Set pH probe
  pinMode(PH_READ_PIN, INPUT);
  
  // Set arrow keys
  for (int i = 0; i < sizeof(arrowPins) / sizeof(arrowPins[0]); i++) {
    pinMode(arrowPins[i], INPUT_PULLDOWN);
    attachInterrupt(arrowPins[i], buttonInterruptHandlers[i], CHANGE);
  }
}
 
void setup() {
  Serial.begin(115200);
  setupPins();
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN); // Initialize Wire with custom I2C SDA and SCL pins
  preferences.begin("settings", false);
  ph.pHAlarmTriggerVal = preferences.getFloat("pHAlarmVal", 4);
  ph.ECAlarmTriggerVal = preferences.getFloat("ECAlarmVal", 4);
  ui.setupDisplay();
}

void loop() {
  ui.clearDisplay();

  if (upPressed)
  {
    sm.transition(0);
    lastInputTime = millis();
    upPressed = false;
  }
  if (downPressed)
  {
    sm.transition(1);
    lastInputTime = millis();
    downPressed = false;
  }
  if (leftPressed)
  {
    sm.transition(2);
    lastInputTime = millis();    
    leftPressed = false;
  }
  if (rightPressed)
  {
    sm.transition(3);
    lastInputTime = millis();    
    rightPressed = false;
  }
  stateMachine();
  ui.updateDisplay();
}