#include <Arduino.h>
#include "ColourSensorHandler.h"

bool ColourSensorHandler::setColour(int colour)
{
  switch(colour)
  {
    case 0: // Red
      digitalWrite(s2, LOW);
      digitalWrite(s3, LOW);
      return true;

    case 1: // Blue
      digitalWrite(s2, LOW);
      digitalWrite(s3, HIGH);
      return true;

    case 2: // No Filter
      digitalWrite(s2, HIGH);
      digitalWrite(s3, LOW);
      return true;

    case 3: // Green
      digitalWrite(s2, HIGH);
      digitalWrite(s3, HIGH);
      return true;
    
    default:
      return false;
  }
}

int ColourSensorHandler::getReading()
{
  noInterrupts();
  int length = pulseIn(out, LOW);
  interrupts();
  return length;
}
