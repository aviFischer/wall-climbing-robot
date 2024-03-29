#include "AccelerometerHandler.h"

bool AccelerometerHandler::takeReading()
{
  return inertialMeasurementUnit.Read();
}
  
int AccelerometerHandler::getOrientation()
{
  float x_acceleration = inertialMeasurementUnit.accel_x_mps2();
  float y_acceleration = inertialMeasurementUnit.accel_y_mps2();
  float z_acceleration = inertialMeasurementUnit.accel_z_mps2();

  if(abs(x_acceleration) >= abs(y_acceleration) && abs(x_acceleration) >= abs(z_acceleration))
  {
    // x axis oriented
    if(x_acceleration > 0)
    {
      return 4;
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
      return 0;
    }
    else
    {
      return 5;
    }
  }

  return -1;
}

// Returns the angle the pitch of the robot inshallah
int AccelerometerHandler::getXZAngle()
{
  float x_acceleration = inertialMeasurementUnit.accel_x_mps2();
  float z_acceleration = inertialMeasurementUnit.accel_z_mps2();

  float percentz = z_acceleration / (z_acceleration + x_acceleration);

  return (int) (percentz * 90);
}

int AccelerometerHandler::getLastReading()
{
  return lastReading;
}

void AccelerometerHandler::bumpLastReading()
{
  lastReading = millis();
}
