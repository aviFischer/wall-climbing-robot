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

void DriveHandler::turn(int duty_cycle, int direction)
{
  if(direction = 1) // clockwise
  {
    digitalWrite(left_motor_direction_in1, HIGH);
    digitalWrite(left_motor_direction_in2, LOW);
    digitalWrite(right_motor_direction_in1, LOW);
    digitalWrite(right_motor_direction_in2, HIGH);
  } else {
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

void DriveHandler::stop()
{
  analogWrite(front_left_motor_pwm, 0);
  analogWrite(back_left_motor_pwm, 0);
  analogWrite(front_right_motor_pwm, 0);
  analogWrite(back_right_motor_pwm, 0);
}

// Returns angle of robot clockwise from starting position
float DriveHandler::getRobotAngle()
{
  // Using left + right because the wheel spin is opposite directions
  float distance_difference = (leftEncoder->getCount() + rightEncoder->getCount()) * count_to_distance;
  float angle = distance_difference / (robot_width * PI) * 360;
  return angle;
}

// Starts driving, polls until sufficient distance is travelled
void DriveHandler::driveUntil(int duty_cycle, int direction, int distance)
{

  // Judging distance purely off left encoder for now
  int initialDistance = rightEncoder->getCount() * count_to_distance;

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

  while(abs(rightEncoder->getCount() * count_to_distance - initialDistance) < abs(distance))
  {}

  stop();
}

void DriveHandler::turnUntil(int duty_cycle, int direction, int angle)
{
  int initialAngle = getRobotAngle();

  if(direction = 1) // clockwise
  {
    digitalWrite(left_motor_direction_in1, HIGH);
    digitalWrite(left_motor_direction_in2, LOW);
    digitalWrite(right_motor_direction_in1, LOW);
    digitalWrite(right_motor_direction_in2, HIGH);
  } else {
    digitalWrite(left_motor_direction_in1, LOW);
    digitalWrite(left_motor_direction_in2, HIGH);
    digitalWrite(right_motor_direction_in1, HIGH);
    digitalWrite(right_motor_direction_in2, LOW);
  }
  analogWrite(front_left_motor_pwm, duty_cycle);
  analogWrite(back_left_motor_pwm, duty_cycle);
  analogWrite(front_right_motor_pwm, duty_cycle);
  analogWrite(back_right_motor_pwm, duty_cycle);

  while(abs(getRobotAngle() - initialAngle) < abs(angle))
  {}

  stop();
}
