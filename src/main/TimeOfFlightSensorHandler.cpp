#include "TimeOfFlightSensorHandler.h"

int TimeOfFlightSensorHandler::getDistance()
{
  ToFSensor.rangingTest(&measure, false);
  if(measure.RangeStatus == 4)
  {
    return -1;
  }
  else
  {
    return measure.RangeMilliMeter;
  }
}
