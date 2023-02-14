#include "DriveHandler.h"

#include <Arduino.h>

void DriveHandler::drive(int duty_cycle, int direction)
{
  if(direction = 1) // forward
  {
    digitalWrite(left_motor_direction_in1, HIGH);
    digitalWrite(left_motor_direction_in2, LOW);
    digitalWrite(right_motor_direction_in1, HIGH);
    digitalWrite(right_motor_direction_in2, LOW);
  } else {
    digitalWrite(left_motor_direction_in1, LOW);
    digitalWrite(left_motor_direction_in2, HIGH);
    digitalWrite(right_motor_direction_in1, LOW);
    digitalWrite(right_motor_direction_in2, HIGH);
  }
  analogWrite(front_left_motor_pwm, duty_cycle);
  analogWrite(back_left_motor_pwm, duty_cycle);
  analogWrite(front_right_motor_pwm, duty_cycle);
  analogWrite(back_right_motor_pwm, duty_cycle);
}

void DriveHandler::stop()
{
  analogWrite(front_left_motor_pwm, 0);
  analogWrite(back_left_motor_pwm, 0);
  analogWrite(front_right_motor_pwm, 0);
  analogWrite(back_right_motor_pwm, 0);
}

void DriveHandler::turn(int duty_cycle, int direction)
{
  if(direction = 1) // CCW
  {
    digitalWrite(left_motor_direction_in1, HIGH);
    digitalWrite(left_motor_direction_in2, LOW);
    digitalWrite(right_motor_direction_in1, LOW);
    digitalWrite(right_motor_direction_in2, HIGH);
  } else { // CW
    digitalWrite(left_motor_direction_in1, LOW);
    digitalWrite(left_motor_direction_in2, HIGH);
    digitalWrite(right_motor_direction_in1, HIGH);
    digitalWrite(right_motor_direction_in2, LOW);
  }
  analogWrite(front_left_motor_pwm, duty_cycle);
  analogWrite(back_left_motor_pwm, duty_cycle);
  analogWrite(front_right_motor_pwm, duty_cycle);
  analogWrite(back_right_motor_pwm, duty_cycle);
}
