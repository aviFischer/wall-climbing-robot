#include "EncoderHandler.h"
#include "ColourSensorHandler.h"
#include "UltrasonicSensorHandler.h"

// Pin Definitions
#define ENCODER_LEFT_A 5
#define ENCODER_LEFT_B 6
#define ENCODER_RIGHT_A 7
#define ENCODER_RIGHT_B 8

#define COLOUR_SENSOR_S2 9
#define COLOUR_SENSOR_S3 10
#define COLOUR_SENSOR_OUT 11

#define ULTRASONIC_SENSOR_TRIGGER 12
#define ULTRASONIC_SENSOR_ECHO 13

// Miscellanious Constants
#define WHEEL_RADIUS 50
#define COUNT_PER_REV 14

// Classes to keep track of sensor data
EncoderHandler *left_encoder;
EncoderHandler *right_encoder;
ColourSensorHandler *colour_sensor;
UltrasonicSensorHandler *ultrasonic_sensor;

// Encoder pin states so PCI can figure which pin changed
volatile int D2_state = LOW;
volatile int D3_state = LOW;
volatile int D4_state = LOW;
volatile int D5_state = LOW;

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
  // put your main code here, to run repeatedly:

}
