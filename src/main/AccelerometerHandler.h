#ifndef ACCELEROMETER_HANDLER_H
#define ACCELEROMETER_HADNLER_H

#include <Arduino.h>
#include "mpu9250.h"

class AccelerometerHandler
{
  bfs::Mpu9250 inertialMeasurementUnit = bfs::Mpu9250(&Wire, 0x68);

  int lastReading;

public:
  AccelerometerHandler()
  { 
    inertialMeasurementUnit.Begin();
    lastReading = 0;
  }

  bool takeReading();
  
  int getOrientation();
  int getXZAngle();

  int getLastReading();
  void bumpLastReading();
};

#endif