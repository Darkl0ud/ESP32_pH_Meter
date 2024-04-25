#include <ui.h>
#include <pH.h>
#include <Preferences.h>

UI ui;
pH ph;
Preferences preferences;

enum class States {
    INFO,
    ALARM,
    SELECTPHALARM,
    SELECTECALARM,
    SETPHALARM,
    SETECALARM,
    CAL,
    SYS,
};

class StateMachine {
public:
    StateMachine() : currentState(States::INFO) {}

    // Function to transition to the next state based on button press
    void transition(int buttonPress) {
            
        currentState = nextStateFunc[static_cast<int>(currentState)](buttonPress);
    }

    // Getter function to retrieve the current state
    States getCurrentState() const {
        return currentState;
    }

private:
    States currentState;
    // Declaration of array of function pointers for state transitions
    static States (*nextStateFunc[])(int);

    // State transition functions
    static States nextStateInfo(int buttonPress) {
        if (buttonPress == 0) {
            return States::SYS;
        } else if (buttonPress == 1) {
            return States::ALARM;
        }
        return States::INFO;
    }

    static States nextStateAlarm(int buttonPress) {
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

    static States nextStateSelectpHAlarm(int buttonPress) {
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

    static States nextStateSelectECAlarm(int buttonPress) {
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

    static States nextStateSetpHAlarm(int buttonPress) {
        if (buttonPress == 0) {
          ph.pHAlarmTriggerVal += 0.1;
        } else if (buttonPress == 1) {
          ph.pHAlarmTriggerVal -= 0.1;
        } else if (buttonPress == 2) {
            float test = preferences.putFloat("pHAlarmVal", ph.pHAlarmTriggerVal);
          return States::SELECTPHALARM;
        } else if (buttonPress == 3) {
        }
        return States::SETPHALARM;
    }

    static States nextStateSetECAlarm(int buttonPress) {
        if (buttonPress == 0) {
          ph.ECAlarmTriggerVal += 0.1;
        } else if (buttonPress == 1) {
          ph.ECAlarmTriggerVal -= 0.1;
        } else if (buttonPress == 2) {
          preferences.putFloat("ECAlarmVal", ph.ECAlarmTriggerVal);
          return States::SELECTECALARM;
        } else if (buttonPress == 3) {
        }
        return States::SETECALARM;
    }

    static States nextStateCal(int buttonPress) {
        if (buttonPress == 0) {
            return States::ALARM;
        } else if (buttonPress == 1) {
            return States::SYS;
        } else if (buttonPress == 2) {
        } else if (buttonPress == 3) {
        }
        return States::CAL;
    }

    static States nextStateSys(int buttonPress) {
        if (buttonPress == 0) {
            return States::CAL;
        } else if (buttonPress == 1) {
            return States::INFO;
        } else if (buttonPress == 2) {
        } else if (buttonPress == 3) {
        }
        return States::SYS;
    }
};

// Define array of function pointers for state transitions
States (*StateMachine::nextStateFunc[])(int) = {
    StateMachine::nextStateInfo,
    StateMachine::nextStateAlarm,
    StateMachine::nextStateSelectpHAlarm,
    StateMachine::nextStateSelectECAlarm,
    StateMachine::nextStateSetpHAlarm,
    StateMachine::nextStateSetECAlarm,
    StateMachine::nextStateCal,
    StateMachine::nextStateSys,
};

StateMachine sm;

void stateMachine(){
    switch (sm.getCurrentState())
  {
  case States::INFO:
    ui.drawMainMenu(0);
    ui.drawpH(ph.getAveragepH(PH_READ_PIN));
    break;
  
  case States::ALARM:
    ui.drawMainMenu(1);
    ui.drawpHSubMenu();
    ui.drawECSubMenu();
    ui.drawpHAlarm(ph.pHAlarmTriggerVal);
    ui.drawECAlarm(ph.ECAlarmTriggerVal);
    break;
  
  case States::CAL:
    ui.drawMainMenu(2);
    break;
  
  case States::SYS:
    ui.drawMainMenu(3);
    break;

  case States::SELECTPHALARM:
    ui.drawMainMenu(1);
    ui.drawpHAlarm(ph.pHAlarmTriggerVal);
    ui.drawECSubMenu();
    ui.drawECAlarm(ph.ECAlarmTriggerVal);

    //Flash currently selected submenu
    if (millis() > ph.flashTimer + 200)
    {
      ui.drawpHSubMenu();
      if (millis() > ph.flashTimer + 400)
      {
        ph.flashTimer = millis();
      }
    }
    break;

  case States::SELECTECALARM:
    ui.drawMainMenu(1);
    ui.drawpHAlarm(ph.pHAlarmTriggerVal);
    ui.drawpHSubMenu();
    ui.drawECAlarm(ph.ECAlarmTriggerVal);

    //Flash currently selected submenu
    if (millis() > ph.flashTimer + 200)
    {
      ui.drawECSubMenu();
      if (millis() > ph.flashTimer + 400)
      {
        ph.flashTimer = millis();
      }
    }
    break;

  case States::SETPHALARM:
    ui.drawMainMenu(1);
    ui.drawECSubMenu();
    ui.drawpHSubMenu();
    ui.drawECAlarm(ph.ECAlarmTriggerVal);
    //Flash currently set pH alarm trigger value
    if (millis() > ph.flashTimer + 200)
    {
    ui.drawpHAlarm(ph.pHAlarmTriggerVal);
  
      if (millis() > ph.flashTimer + 400)
      {
        ph.flashTimer = millis();
      }
    } 
    break;

  case States::SETECALARM:
    ui.drawMainMenu(1);
    ui.drawECSubMenu();
    ui.drawpHSubMenu();
    ui.drawpHAlarm(ph.pHAlarmTriggerVal);
    //Flash currently set EC alarm trigger value
    if (millis() > ph.flashTimer + 200)
    {
    ui.drawECAlarm(ph.ECAlarmTriggerVal);
  
      if (millis() > ph.flashTimer + 400)
      {
        ph.flashTimer = millis();
      }
    }
    break;   

  default:
    break;
  }
}