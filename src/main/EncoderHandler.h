#ifndef ENCODER_HANDLER_H
#define ENCODER_HANDLER_H

class EncoderHandler
{
  volatile int counter;
  volatile int a_state;
  volatile int b_state;

  double radius;
  int count_per_revolution;

public:
  EncoderHandler(int a, int b, double radius, int count_per_revolution) {
    a_state = a;
    b_state = b;

    this->radius = radius;
    this->count_per_revolution = count_per_revolution;
  }

  void toggle_a();
  void toggle_b();

  double distance();
  int getCount();
};

#endif