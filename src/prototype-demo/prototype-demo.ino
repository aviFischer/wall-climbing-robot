#import "UltrasonicSensorHandler.h"
#import "DriveHandler.h"
#import "AccelerometerHandler.h"

#define LEFT_MOTORS_DIRECTION_IN1 2
#define LEFT_MOTORS_DIRECTION_IN2 4
#define RIGHT_MOTORS_DIRECTION_IN1 7
#define RIGHT_MOTORS_DIRECTION_IN2 8

#define FRONT_LEFT_MOTOR_PWM 3
#define BACK_LEFT_MOTOR_PWM 5
#define FRONT_RIGHT_MOTOR_PWM 6
#define BACK_RIGHT_MOTOR_PWM 9

#define ULTRASONIC_SENSOR_TRIGGER 11
#define ULTRASONIC_SENSOR_ECHO 10

#define LED_PIN 12

DriveHandler *drive_handler;
UltrasonicSensorHandler *ultrasonic_sensor;
AccelerometerHandler *accelerometer;

int state = 1;
long start_time;

void setup()
{
  DriveHandler::motorControllerPins_t motor_pins = {
    LEFT_MOTORS_DIRECTION_IN1,
    LEFT_MOTORS_DIRECTION_IN2,
    RIGHT_MOTORS_DIRECTION_IN1,
    RIGHT_MOTORS_DIRECTION_IN2,

    FRONT_LEFT_MOTOR_PWM,
    BACK_LEFT_MOTOR_PWM,
    FRONT_RIGHT_MOTOR_PWM,
    BACK_RIGHT_MOTOR_PWM
  };

  // setup motor control pins
  pinMode(LEFT_MOTORS_DIRECTION_IN1, OUTPUT);
  pinMode(LEFT_MOTORS_DIRECTION_IN2, OUTPUT);
  pinMode(RIGHT_MOTORS_DIRECTION_IN1, OUTPUT);
  pinMode(RIGHT_MOTORS_DIRECTION_IN2, OUTPUT);
  pinMode(FRONT_LEFT_MOTOR_PWM, OUTPUT);
  pinMode(BACK_LEFT_MOTOR_PWM, OUTPUT);
  pinMode(FRONT_RIGHT_MOTOR_PWM, OUTPUT);
  pinMode(BACK_RIGHT_MOTOR_PWM, OUTPUT);

  drive_handler = new DriveHandler(motor_pins);

  // Setup ultrasonic sensor pins
  pinMode(ULTRASONIC_SENSOR_TRIGGER, OUTPUT);
  pinMode(ULTRASONIC_SENSOR_ECHO, INPUT);

  ultrasonic_sensor = new UltrasonicSensorHandler(ULTRASONIC_SENSOR_TRIGGER, ULTRASONIC_SENSOR_ECHO);

  pinMode(LED_PIN, OUTPUT);

  accelerometer = new AccelerometerHandler;

  Serial.begin(9600);

  start_time = millis();
}

void loop()
{

  //Testing ultrasonic sensor input with two readings
  int dist1 = ultrasonic_sensor->getDuration() * 0.0343 / 2;
  int dist2 = ultrasonic_sensor->getDuration() * 0.0343 / 2;

  // if within 10cm of wall stop moving
  if(dist1 < 100 || dist2 < 100)
  {
    digitalWrite(LED_PIN, HIGH);
    drive_handler->stop();
  }
  else if(state == 1) // drive forward
  {
    digitalWrite(LED_PIN, LOW);
    if(millis() - start_time > 5000)
    {
      state = 2;
      start_time = millis();
    }
    else
    {
      drive_handler->drive(50, 1);
    }
  }
  else if(state == 2) // drive backwards
  {
    digitalWrite(LED_PIN, LOW);
    if(millis() - start_time > 5000)
    {
      state = 3;
      start_time = millis();
    }
    else
    {
      drive_handler->drive(50, -1);
    }
  }
  else if(state == 3) // turn ccw
  {
    digitalWrite(LED_PIN, LOW);
    if(millis() - start_time > 500)
    {
      state = 4;
      start_time = millis();
    }
    else
    {
      drive_handler->turn(50, 1);
    }
  }
  else if(state == 4) // turn CW
  {
    digitalWrite(LED_PIN, LOW);
    if(millis() - start_time > 500)
    {
      state = 1;
      start_time = millis();
    }
    else
    {
      drive_handler->turn(50, -1);
    }
  }

  Serial.println(accelerometer->getOrientation());
  delay(100);
}
