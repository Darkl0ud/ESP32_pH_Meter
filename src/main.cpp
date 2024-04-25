#include <main.h>

UI ui;
pH ph;

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
  //Serial.begin(115200);
  setupPins();
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN); // Initialize Wire with custom I2C SDA and SCL pins
  ui.setupDisplay();
}

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
  } else if (buttonPress == 2) {
  } else if (buttonPress == 3) {
    return States::SELECTPHALARM;
  }
  return States::ALARM;
}

States nextStateSelectpHAlarm(int buttonPress) {
  if (buttonPress == 0) {
    return States::SELECTECALARM;
  } else if (buttonPress == 1) {
    return States::SELECTECALARM;
  } else if (buttonPress == 2) {
    return States::ALARM;
  } else if (buttonPress == 3) {
    return States::SETPHALARM;
  }
  return States::SELECTPHALARM;
}

States nextStateSelectECAlarm(int buttonPress) {
  if (buttonPress == 0) {
    return States::SELECTPHALARM;
  } else if (buttonPress == 1) {
    return States::SELECTPHALARM;
  } else if (buttonPress == 2) {
    return States::ALARM;
  } else if (buttonPress == 3) {
    return States::SETECALARM;
  }
  return States::SELECTECALARM;
}

States nextStateSetpHAlarm(int buttonPress) {
  if (buttonPress == 0) {
    pHAlarmTriggerVal += 0.1;
  } else if (buttonPress == 1) {
    pHAlarmTriggerVal -= 0.1;
  } else if (buttonPress == 2) {
    return States::SELECTPHALARM;
  } else if (buttonPress == 3) {
  }
  return States::SETPHALARM;
}

States nextStateSetECAlarm(int buttonPress) {
  if (buttonPress == 0) {
    ECAlarmTriggerVal += 0.1;
  } else if (buttonPress == 1) {
    ECAlarmTriggerVal -= 0.1;
  } else if (buttonPress == 2) {
    return States::SELECTECALARM;
  } else if (buttonPress == 3) {
  }
  return States::SETECALARM;
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



// Array of function pointers for state transitions
States (*nextStateFunc[])(int) = {
  nextStateInfo,
  nextStateAlarm,
  nextStateSelectpHAlarm,
  nextStateSelectECAlarm,
  nextStateSetpHAlarm,
  nextStateSetECAlarm,
  nextStateCal,
  nextStateSys,
};

void loop() {
  ui.clearDisplay();

  if (upPressed)
  {
    currentState = nextStateFunc[static_cast<int>(currentState)](0);
    lastInputTime = millis();
    upPressed = false;
  }
  if (downPressed)
  {
    currentState = nextStateFunc[static_cast<int>(currentState)](1);
    lastInputTime = millis();
    downPressed = false;
  }
  if (leftPressed)
  {
    currentState = nextStateFunc[static_cast<int>(currentState)](2);
    lastInputTime = millis();    
    leftPressed = false;
  }
  if (rightPressed)
  {
    currentState = nextStateFunc[static_cast<int>(currentState)](3);
    lastInputTime = millis();    
    rightPressed = false;
  }
  
  switch (currentState)
  {
  case States::INFO:
    ui.drawMainMenu(0);
    ui.drawpH(ph.getAveragepH(PH_READ_PIN));
    break;
  
  case States::ALARM:
    ui.drawMainMenu(1);
    ui.drawpHSubMenu();
    ui.drawECSubMenu();
    ui.drawpHAlarm(pHAlarmTriggerVal);
    ui.drawECAlarm(ECAlarmTriggerVal);
    break;
  
  case States::CAL:
    ui.drawMainMenu(2);
    break;
  
  case States::SYS:
    ui.drawMainMenu(3);
    break;

  case States::SELECTPHALARM:
    ui.drawMainMenu(1);
    ui.drawpHAlarm(pHAlarmTriggerVal);
    ui.drawECSubMenu();
    ui.drawECAlarm(ECAlarmTriggerVal);

    //Flash currently selected submenu
    if (millis() > flashTimer + 200)
    {
      ui.drawpHSubMenu();
      if (millis() > flashTimer + 400)
      {
        flashTimer = millis();
      }
    }
    break;

  case States::SELECTECALARM:
    ui.drawMainMenu(1);
    ui.drawpHAlarm(pHAlarmTriggerVal);
    ui.drawpHSubMenu();
    ui.drawECAlarm(ECAlarmTriggerVal);

    //Flash currently selected submenu
    if (millis() > flashTimer + 200)
    {
      ui.drawECSubMenu();
      if (millis() > flashTimer + 400)
      {
        flashTimer = millis();
      }
    }
    break;

  case States::SETPHALARM:
    ui.drawMainMenu(1);
    ui.drawECSubMenu();
    ui.drawpHSubMenu();
    ui.drawECAlarm(ECAlarmTriggerVal);
    //Flash currently set pH alarm trigger value
    if (millis() > flashTimer + 200)
    {
    ui.drawpHAlarm(pHAlarmTriggerVal);
  
      if (millis() > flashTimer + 400)
      {
        flashTimer = millis();
      }
    }
    break;

  case States::SETECALARM:
    ui.drawMainMenu(1);
    ui.drawECSubMenu();
    ui.drawpHSubMenu();
    ui.drawpHAlarm(pHAlarmTriggerVal);
    //Flash currently set pH alarm trigger value
    if (millis() > flashTimer + 200)
    {
    ui.drawECAlarm(ECAlarmTriggerVal);
  
      if (millis() > flashTimer + 400)
      {
        flashTimer = millis();
      }
    }
    break;   

  default:
    break;
  }
  ui.updateDisplay();
}