#ifndef MOTOR_HANDLER_H
#define MOTOR_HANDLER_H

class DriveHandler
{

  int left_motor_direction_in1;
  int left_motor_direction_in2;
  int right_motor_direction_in1;
  int right_motor_direction_in2;
  int front_left_motor_pwm;
  int back_left_motor_pwm;
  int front_right_motor_pwm;
  int back_right_motor_pwm;

public:
  typedef struct motorControllerPins_t
  {
    int left_motor_direction_in1;
    int left_motor_direction_in2;
    int right_motor_direction_in1;
    int right_motor_direction_in2;
    int front_left_motor_pwm;
    int back_left_motor_pwm;
    int front_right_motor_pwm;
    int back_right_motor_pwm;
  };

  DriveHandler(motorControllerPins_t pins)
  {
    left_motor_direction_in1 = pins.left_motor_direction_in1;
    left_motor_direction_in2 = pins.left_motor_direction_in2;
    right_motor_direction_in1 = pins.right_motor_direction_in1;
    right_motor_direction_in2 = pins.right_motor_direction_in2;
    front_left_motor_pwm = pins.front_left_motor_pwm;
    back_left_motor_pwm = pins.back_left_motor_pwm;
    front_right_motor_pwm = pins.front_right_motor_pwm;
    back_right_motor_pwm = pins.back_right_motor_pwm;
  }

  void drive(int duty_cycle, int direction);
  //void driveUntil(int duty_cycle, int direction);
  void stop();
  void turn(int duty_cycle, int direction);
  //void turnUntil(int duty_cycle, int direction);
};

#endif