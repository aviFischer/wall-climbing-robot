#include <Arduino.h>
#include "UltrasonicSensorHandler.h"

int UltrasonicSensorHandler::getDuration()
{
  noInterrupts();
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  int duration = pulseIn(echo, HIGH);
  interrupts();
  return duration;
}