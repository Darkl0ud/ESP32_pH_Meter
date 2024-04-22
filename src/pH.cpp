#include <pH.h>

// Returns calibrated pH value from input
float pHC::calculateCalibratedpH(float rawAnalog, float pHRangeHigh, float pHRangeLow, 
                            float pHHighCal, float pHLowCal){
  float slope = (pHRangeHigh - pHRangeLow) / (pHHighCal - pHLowCal);
  float intercept = pHRangeHigh - slope * pHHighCal; 
  return slope * rawAnalog + intercept;
}

// Returns calibrated pH value depending on pH range
float pHC::getCalibratedpH(int readPin){
  float rawAnalog = analogRead(readPin) / 4095.0;
  if (rawAnalog >= pH14Cal && rawAnalog <= pH10Cal) {
    pH = calculateCalibratedpH(rawAnalog, 14, 10, pH14Cal, pH10Cal);
  }        

  if (rawAnalog >= pH10Cal && rawAnalog <= pH7Cal)  {
    pH = calculateCalibratedpH(rawAnalog, 10, 7, pH10Cal, pH7Cal);
  }

  if (rawAnalog >= pH7Cal && rawAnalog <= pH4Cal) {
    pH = calculateCalibratedpH(rawAnalog, 7, 4, pH7Cal, pH4Cal);
  }

  if (rawAnalog >= pH4Cal && rawAnalog <= pH0Cal) {  
    pH = calculateCalibratedpH(rawAnalog, 4, 0, pH4Cal, pH0Cal);
  }
  return pH;
}

float pHC::calculateAveragepH(int readPin) {
  unsigned long currentMillis = millis();
  // Once 100ms have passed...
  if (currentMillis - lastpHMillis >= 100)  { 
    // Save the current time.
    lastpHMillis = currentMillis;
    pHAnalogArray[j] = getCalibratedpH(readPin);
    j++;
  }
   
  // Once 1000ms have passed...
  if (currentMillis - lastSecond >= 1000) {  
    lastSecond = currentMillis;
    j = 0;
    char maxIndex = 0;
    char minIndex = 0;

    // Check each reading that we put in the array if our max or min number is higher or lower.
    for (int i = 0; i < 10; i++) {  
      if (pHAnalogArray[i] > pHAnalogArray[maxIndex]) {
        maxIndex = i;
      }

      if (pHAnalogArray[i] < pHAnalogArray[minIndex]) {
        minIndex = i;
      }
    }

    // Set the highest value in the pH array to 0 so it is not counted in average.
    pHAnalogArray[maxIndex] = 0; 
    // Set the lowest value in the pH array to 0 so it is not counted in average.
    pHAnalogArray[minIndex] = 0; 

    // For use with determining average pH over the given 10 readings.
    float Sum = 0;  
    // Holds how many pH readings above 0 we have.
    unsigned char countsOverZero = 0;  
    
    // Check each value in pH array. If not 0, add it to sum and increase count.
    for (int i = 0; i < 10; i++) {  
      if (pHAnalogArray[i] != 0) {
        Sum += pHAnalogArray[i];
        countsOverZero++;
      }
    }

    // Calculate average of pH readings over 1 second
    pHAverage = Sum/countsOverZero;
 }

  return pHAverage;
}