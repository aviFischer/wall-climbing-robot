#include "EncoderHandler.h"

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

int EncoderHandler::getCount()
{
  return counter;
}
