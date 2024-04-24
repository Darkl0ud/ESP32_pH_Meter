#include <main.h>

UI ui;
pH ph;

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

enum class States{
  INFO, 
  ALARM, 
  CAL, 
  SYS,
  SETALARM
};

States currentState = States::INFO;

States nextStateInfo(int buttonPress) {
  if (buttonPress == 0) {
    return States::SYS;
  } else if (buttonPress == 1) {
    return States::ALARM;
  } else if (buttonPress == 2) {
  } else if (buttonPress == 3) {
  }
  return States::INFO;
}

States nextStateAlarm(int buttonPress) {
  if (buttonPress == 0) {
    return States::INFO;
  } else if (buttonPress == 1) {
    return States::CAL;
  }else if (buttonPress == 2) {
  } else if (buttonPress == 3) {
    return States::SETALARM;
  }
  return States::ALARM;
}

States nextStateCal(int buttonPress) {
  if (buttonPress == 0) {
    return States::ALARM;
  } else if (buttonPress == 1) {
    return States::SYS;
  } else if (buttonPress == 2) {
  } else if (buttonPress == 3) {
  }
  return States::CAL;
}

States nextStateSys(int buttonPress) {
  if (buttonPress == 0) {
    return States::CAL;
  } else if (buttonPress == 1) {
    return States::INFO;
  } else if (buttonPress == 2) {
  } else if (buttonPress == 3) {
  }
  return States::SYS;
}

States nextStateSetAlarm(int buttonPress) {
  if (buttonPress == 0) {
  } else if (buttonPress == 1) {
  } else if (buttonPress == 2) {
    return States::ALARM;
  } else if (buttonPress == 3) {
  }
  return States::SETALARM;
}

// Array of function pointers for state transitions
States (*nextStateFunc[])(int) = {
  nextStateInfo,
  nextStateAlarm,
  nextStateCal,
  nextStateSys,
  nextStateSetAlarm
};

void loop() {
  ui.clearDisplay();

  if (upPressed)
  {
    Serial.println("UP");
    currentState = nextStateFunc[static_cast<int>(currentState)](0);
    upPressed = false;
  }
  if (downPressed)
  {
    Serial.println("DOWN");
    currentState = nextStateFunc[static_cast<int>(currentState)](1);
    downPressed = false;
  }
  if (leftPressed)
  {
    Serial.println("LEFT");
    currentState = nextStateFunc[static_cast<int>(currentState)](2);
    leftPressed = false;
  }
  if (rightPressed)
  {
    Serial.println("RIGHT");
    currentState = nextStateFunc[static_cast<int>(currentState)](3);
    rightPressed = false;
  }
  
  switch (currentState)
  {
  case States::INFO:
    ui.drawMainMenu(0);
  break;
  
  case States::ALARM:
    ui.drawMainMenu(1);
    ui.drawpHSubMenu();
  break;
  
  case States::CAL:
    ui.drawMainMenu(2);
  break;
  
  case States::SYS:
    ui.drawMainMenu(3);
  break;

  case States::SETALARM:
    ui.drawMainMenu(1);
    //ui.drawpHSubMenu();
  break;

  default:
    break;
  }
  //Serial.println(int(currentState));
  ui.updateDisplay();
}