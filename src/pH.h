#include <Arduino.h>

// Set to the corresponding pin for the pH sensor
#define PH_READ_PIN 8

 // Time based variables for calculating pH average over time
static unsigned long lastpHMillis = 0;
static unsigned long lastSecond = 0;

class pH {
    
    public:
        float calculateCalibratedpH(float rawAnalog, float pHRangeHigh, float pHRangeLow, 
                                    float pHHighCal, float pHLowCal);
        float getCalibratedpH(int readPin);
        float getAveragepH(int readPin);

        // Variables used or reading pH value and averaging
        float pH = 0;
        float pHAnalogArray[10];   // Array used for pH reading calculations.
        float pHAverage = 0;
        int j = 0;

        // Variables used for pH calibration
        float pH0Cal = 1;  // Default = 1, shouldn't really need to be changed.
        float pH4Cal = 0.622;  // Voltage in pH 4 buffer, default = 0.7146.
        float pH7Cal = 0.5126;  // Voltage in pH 7 buffer, default 0.5.
        float pH10Cal = 0.2865; // Voltage in pH 10 buffer, default 0.2865.
        float pH14Cal = 0;  // Default = 0, shouldn't really need to be changed.

        // pH and EC Alarm Vals
        float ECAlarmTriggerVal = 14.20;
        float pHAlarmTriggerVal = 12.00;
        unsigned long int flashTimer;
    private:
};