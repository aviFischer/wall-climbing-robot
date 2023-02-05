#include "EncoderHandler.h"

#define _USE_MATH_DEFINES
 
#include <math.h>

void EncoderHandler::toggle_a()
{
  a_state = a_state == 1 ? 0 : 1;
  if(a_state != b_state)
  {
    counter ++;
  }
  else 
  {
    counter --;
  }
}

void EncoderHandler::toggle_b()
{
  b_state = b_state == 1 ? 0 : 1;
}

double EncoderHandler::distance()
{
  return counter / count_per_revolution * pow(radius, 2.0) * M_PI;
}

int EncoderHandler::getCount()
{
  return counter;
}
