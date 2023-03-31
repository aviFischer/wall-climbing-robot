#ifndef ENCODER_HANDLER_H
#define ENCODER_HANDLER_H

class EncoderHandler
{
  volatile long counter;
  volatile int a_state;
  volatile int b_state;

  double radius;
  int count_per_revolution;

public:
  EncoderHandler(int a, int b) {
    a_state = a;
    b_state = b;
    counter = 0;

    this->radius = radius;
    this->count_per_revolution = count_per_revolution;
  }

  void toggle_a();
  void toggle_b();

  int getCount();
};

#endif