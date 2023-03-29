#ifndef ToF_SENSOR_HANDLER_H
#define ToF_SENSOR_HANDLER_H

#include "Adafruit_VL53L0X.h"

class TimeOfFlightSensorHandler {

  Adafruit_VL53L0X ToFSensor = Adafruit_VL53L0X();
  VL53L0X_RangingMeasurementData_t measure;

public:
  TimeOfFlightSensorHandler()
  {
    ToFSensor.begin();
  }

  int getDistance();
};

#endif