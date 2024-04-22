#include <main.h>

// Button interrupt functions
void upButtonChanged() {
  if (digitalRead(UP_BUTTON_PIN) == 1)  {
    upPressed = true;
  }
   
  else  {
    upPressed = false;
  }
}

void downButtonChanged() {
  if (digitalRead(DOWN_BUTTON_PIN) == 1)  {
    downPressed = true;
  }

  else  {
    downPressed = false;
  }
}

void leftButtonChanged() {
  if (digitalRead(LEFT_BUTTON_PIN) == 1)  {
    leftPressed = true;
  }

  else  {
    leftPressed = false;
  }
}

void rightButtonChanged() {
  if (digitalRead(RIGHT_BUTTON_PIN) == 1)  {
    rightPressed = true;
  }

  else  {
    rightPressed = false;
  }
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
  ui.setupDisplay();
}

void loop() {
  ui.updateDisplay(phc.calculateAveragepH(PH_READ_PIN));
}