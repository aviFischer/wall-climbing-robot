#include "TimeOfFlightSensorHandler.h"

int TimeOfFlightSensorHandler::getDistance()
{
  ToFSensor.rangingTest(&measure, false);
  if(measure.RangeStatus == 4)
  {
    return 8191;
  }
  else
  {
    return measure.RangeMilliMeter;
  }
}
