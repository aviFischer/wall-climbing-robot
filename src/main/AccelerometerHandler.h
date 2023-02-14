#ifndef ACCELEROMETER_HANDLER_H
#define ACCELEROMETER_HADNLER_H

#include <Arduino.h>
#include "mpu9250.h"

class AccelerometerHandler
{
  bfs::Mpu9250 inertialMeasurementUnit = bfs::Mpu9250(&Wire, 0x68);

public:
  AccelerometerHandler()
  {
    Wire.begin();
    Wire.setClock(400000);
    inertialMeasurementUnit.Begin();
  }

  bool takeReading();
  
  int getOrientation();
};

#endif