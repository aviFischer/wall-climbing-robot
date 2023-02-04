#include "EncoderHandler.h"

// Pin Definitions
#define ENCODER_LEFT_A 5
#define ENCODER_LEFT_B 6
#define ENCODER_RIGHT_A 7
#define ENCODER_RIGHT_B 8

// Miscellanious Constants
#define WHEEL_RADIUS 50
#define COUNT_PER_REV 14

// Classes to keep track of encoder data
EncoderHandler left_encoder;
EncoderHandler right_encoder;

// Pin states so PCI can figure which pin changed
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

}

ISR(PCINT2_vect)
{
  // figuring out which pin triggered isr
  if(digitalRead(ENCODER_LEFT_A) != D2_state)
  {
    D2_state = D2_state == LOW ? HIGH : LOW;
    left_encoder.toggle_a();
    return
  }
  if(digitalRead(ENCODER_LEFT_B) != D3_state)
  {
    D3_state = D3_state == LOW ? HIGH : LOW;
    left_encoder.toggle_b();
    return
  }
  if(digitalRead(ENCODER_RIGHT_A) != D4_state)
  {
    D4_state = D4_state == LOW ? HIGH : LOW;
    right_encoder.toggle_a();
    return
  }
  if(digitalRead(ENCODER_RIGHT_B) != D5_state)
  {
    D5_state = D5_state == LOW ? HIGH : LOW;
    right_encoder.toggle_b();
    return
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
