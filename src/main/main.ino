#include "DriveHandler.h"
#include "EncoderHandler.h"
#include "ColourSensorHandler.h"
#include "UltrasonicSensorHandler.h"
#include "AccelerometerHandler.h"

#include <SparkFun_MMA8452Q.h>

// Pin Definitions
#define ENCODER_LEFT_A 5
#define ENCODER_LEFT_B 6
#define ENCODER_RIGHT_A 26
#define ENCODER_RIGHT_B 25

#define COLOUR_SENSOR_S2 14
#define COLOUR_SENSOR_S3 15
#define COLOUR_SENSOR_OUT 7

#define ULTRASONIC_SENSOR_TRIGGER 11
#define ULTRASONIC_SENSOR_ECHO 10

#define LEFT_MOTORS_DIRECTION_IN1 19
#define LEFT_MOTORS_DIRECTION_IN2 20
#define RIGHT_MOTORS_DIRECTION_IN1 21
#define RIGHT_MOTORS_DIRECTION_IN2 22

#define FRONT_LEFT_MOTOR_PWM 8
#define BACK_LEFT_MOTOR_PWM 9
#define FRONT_RIGHT_MOTOR_PWM 12
#define BACK_RIGHT_MOTOR_PWM 13

#define GYRO_SDA 23
#define GYRO_SCL 24

// Miscellanious Constants
#define WHEEL_RADIUS 50
#define COUNT_PER_REV 14

#define THRESHOLD 10

// Classes to keep track of sensor data
EncoderHandler *left_encoder;
EncoderHandler *right_encoder;
ColourSensorHandler *colour_sensor;
UltrasonicSensorHandler *ultrasonic_sensor;
DriveHandler *drive_handler;

// Encoder pin states so PCI can figure which pin changed
volatile int D2_state = LOW;
volatile int D3_state = LOW;
volatile int D4_state = LOW;
volatile int D5_state = LOW;

int state = 0;

void setup() {
  // Setting up pin change interrupts to process encoders
  PCICR |= 0b00000100; // enables PCI for post D on atmega328
  PCMSK2 |= 0b00111100; // Trigger on pins D2-D5, the four pins needed for two endcoders

  // Setting up encoder input pins
  pinMode(ENCODER_LEFT_A, INPUT);
  pinMode(ENCODER_LEFT_B, INPUT);
  pinMode(ENCODER_RIGHT_A, INPUT);
  pinMode(ENCODER_RIGHT_B, INPUT);

  // Getting initial encoder states
  D2_state = digitalRead(ENCODER_LEFT_A);
  D3_state = digitalRead(ENCODER_LEFT_B);
  D4_state = digitalRead(ENCODER_RIGHT_A);
  D5_state = digitalRead(ENCODER_RIGHT_B);

  // Initialize encoder handlers
  left_encoder = new EncoderHandler(D2_state, D3_state, WHEEL_RADIUS, COUNT_PER_REV);
  right_encoder = new EncoderHandler(D4_state, D5_state, WHEEL_RADIUS, COUNT_PER_REV);

  // Setup colour sensor Pins
  pinMode(COLOUR_SENSOR_S2, OUTPUT);
  pinMode(COLOUR_SENSOR_S3, OUTPUT);
  pinMode(COLOUR_SENSOR_OUT, INPUT);

  // Initialize colour sensor handler
  colour_sensor = new ColourSensorHandler(COLOUR_SENSOR_S2, COLOUR_SENSOR_S3, COLOUR_SENSOR_OUT);

  // Setup ultrasonic sensor pins
  pinMode(ULTRASONIC_SENSOR_TRIGGER, OUTPUT);
  pinMode(ULTRASONIC_SENSOR_ECHO, INPUT);

  // Initialize ultrasonic sensor handler
  ultrasonic_sensor = new UltrasonicSensorHandler(ULTRASONIC_SENSOR_TRIGGER, ULTRASONIC_SENSOR_ECHO);

  // setup motor control pins
  pinMode(LEFT_MOTORS_DIRECTION_IN1, OUTPUT);
  pinMode(LEFT_MOTORS_DIRECTION_IN2, OUTPUT);
  pinMode(RIGHT_MOTORS_DIRECTION_IN1, OUTPUT);
  pinMode(RIGHT_MOTORS_DIRECTION_IN2, OUTPUT);
  pinMode(FRONT_LEFT_MOTOR_PWM, OUTPUT);
  pinMode(BACK_LEFT_MOTOR_PWM, OUTPUT);
  pinMode(FRONT_RIGHT_MOTOR_PWM, OUTPUT);
  pinMode(BACK_RIGHT_MOTOR_PWM, OUTPUT);

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

  // initialize drive handler
  drive_handler = new DriveHandler(motor_pins, left_encoder, right_encoder);

}

ISR(PCINT2_vect)
{
  // figuring out which pin triggered isr
  if(digitalRead(ENCODER_LEFT_A) != D2_state)
  {
    D2_state = D2_state == LOW ? HIGH : LOW;
    left_encoder->toggle_a();
    return;
  }
  if(digitalRead(ENCODER_LEFT_B) != D3_state)
  {
    D3_state = D3_state == LOW ? HIGH : LOW;
    left_encoder->toggle_b();
    return;
  }
  if(digitalRead(ENCODER_RIGHT_A) != D4_state)
  {
    D4_state = D4_state == LOW ? HIGH : LOW;
    right_encoder->toggle_a();
    return;
  }
  if(digitalRead(ENCODER_RIGHT_B) != D5_state)
  {
    D5_state = D5_state == LOW ? HIGH : LOW;
    right_encoder->toggle_b();
    return;
  }
}

void loop() {
  
  // If elsing to get to appropriate state
  if(state == 0)
  {
    // driving to wall
    drive_handler->drive(150, 1);
    // Use ultrasonic Sensor to detect being close to wall
    // Taking two readings to deal with uncertainty
    int duration1 = ultrasonic_sensor->getDuration();
    int duration2 = ultrasonic_sensor->getDuration();

    if(duration1 < THRESHOLD && duration2 < THRESHOLD)
    {
      state = 1;
    }
  }
  else if(state == 1)
  {
    // driving with full power to get over the wall
    drive_handler->drive(255, 1);
  }
  else if(state == 2)
  {
    // driving down with minimal power
  }

}
