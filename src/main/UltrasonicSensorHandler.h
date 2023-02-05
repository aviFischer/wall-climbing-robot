#ifndef ULTRASONIC_SENSOR_HANDLER_H
#define ULTRASONIC_SENSOR_HANDLER_H

class UltrasonicSensorHandler {
  int trigger;
  int echo;

public:
  UltrasonicSensorHandler(int trigger, int echo)
  {
    this->trigger = trigger;
    this->echo = echo;
  }

  int getDuration();
};

#endif