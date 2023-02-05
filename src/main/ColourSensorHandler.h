#ifndef COLOUR_SENSOR_HANDLER_H
#define COLOUR_SENSOR_HANDLER_H

class ColourSensorHandler {
  int s2;
  int s3;
  int out;

public:
  ColourSensorHandler(int s2, int s3, int out)
  {
    this->s2 = s2;
    this->s3 = s3;
    this->out = out;
  }

  bool setColour(int colour);
  int getReading();
};

#endif