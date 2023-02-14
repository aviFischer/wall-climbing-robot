#include "AccelerometerHandler.h"

bool AccelerometerHandler::bool takeReading()
{
  return inertialMeasurementUnit.Read();
}
  
int AccelerometerHandler::int getOrientation()
{
  inertialMeasurementUnit.Read();
  float x_acceleration inertialMeasurementUnit.accel_x_mps2();
  float y_acceleration inertialMeasurementUnit.accel_y_mps2();
  float z_acceleration inertialMeasurementUnit.accel_z_mps2();

  if(abs(x_acceleration) >= abs(y_acceleration) && abs(x_acceleration) >= abs(z_acceleration))
  {
    // x axis oriented
    if(x_acceleration > 0)
    {
      return 0;
    }
    else
    {
      return 1;
    }
  }
  else if(abs(y_acceleration) >= abs(x_acceleration) && abs(y_acceleration) >= abs(z_acceleration))
  {
    // y axis oreinted
    if(y_acceleration > 0)
    {
      return 2;
    }
    else
    {
      return 3;
    }
  }
  else
  {
    // z axis oriented
    if(z_acceleration > 0)
    {
      return 4;
    }
    else
    {
      return 5;
    }
  }

  return -1;
}