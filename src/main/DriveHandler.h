#ifndef MOTOR_HANDLER_H
#define MOTOR_HANDLER_H

#include "EncoderHandler.h"

class DriveHandler
{

  EncoderHandler *leftEncoder;
  EncoderHandler *rightEncoder;
  int left_motor_direction_in1;
  int left_motor_direction_in2;
  int right_motor_direction_in1;
  int right_motor_direction_in2;
  int front_left_motor_pwm;
  int back_left_motor_pwm;
  int front_right_motor_pwm;
  int back_right_motor_pwm;
  int wheel_radius;
  int robot_width;
  float count_to_distance;

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

  typedef struct motorDimensions_t
  {
    int wheel_radius;
    int robot_width;
    float count_to_distance;
  };

  DriveHandler(motorControllerPins_t pins, motorDimensions_t dimensions, EncoderHandler *leftEncoder, EncoderHandler *rightEncoder)
  {
    left_motor_direction_in1 = pins.left_motor_direction_in1;
    left_motor_direction_in2 = pins.left_motor_direction_in2;
    right_motor_direction_in1 = pins.right_motor_direction_in1;
    right_motor_direction_in2 = pins.right_motor_direction_in2;
    front_left_motor_pwm = pins.front_left_motor_pwm;
    back_left_motor_pwm = pins.back_left_motor_pwm;
    front_right_motor_pwm = pins.front_right_motor_pwm;
    back_right_motor_pwm = pins.back_right_motor_pwm;

    wheel_radius = dimensions.wheel_radius;
    robot_width = dimensions.robot_width;
    count_to_distance = dimensions.count_to_distance;

    this->leftEncoder = leftEncoder;
    this->rightEncoder = rightEncoder;
  }

  void drive(int duty_cycle, int direction);
  void driveUntil(int duty_cycle, int direction, int distance);
  void stop();
  float getRobotAngle();
  void turn(int duty_cycle, int direction);
  void turnUntil(int duty_cycle, int direction, int angle);
};

#endif