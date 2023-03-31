#include "DriveHandler.h"
#include "EncoderHandler.h"
#include "TimeOfFlightSensorHandler.h"

// Pin Definitions
#define ENCODER_LEFT_A 2
#define ENCODER_LEFT_B 3 
#define ENCODER_RIGHT_A 4
#define ENCODER_RIGHT_B 5

#define LEFT_MOTORS_DIRECTION_IN1 7 
#define LEFT_MOTORS_DIRECTION_IN2 8
#define RIGHT_MOTORS_DIRECTION_IN1 12 
#define RIGHT_MOTORS_DIRECTION_IN2 13

#define FRONT_LEFT_MOTOR_PWM 6
#define BACK_LEFT_MOTOR_PWM 9
#define FRONT_RIGHT_MOTOR_PWM 10
#define BACK_RIGHT_MOTOR_PWM 11

#define SDA A4
#define SCL A5

#define LED0 A1
#define LED1 A2
#define LED2 A3

// Miscellanious Constants
#define WHEEL_RADIUS 40
#define COUNT_PER_REV 14
#define CIRCLE_DIAMETER 240
#define GEAR_REDUCTION 210

#define COUNT_TO_DISTANCE (WHEEL_RADIUS * 2 * PI / (COUNT_PER_REV * GEAR_REDUCTION))

#define MOTOR_FULL_POWER 255
#define MOTOR_POWER_FOR_TEST 200
#define MOTOR_HALF_POWER 127

// Classes to keep track of sensor data
EncoderHandler *left_encoder;
EncoderHandler *right_encoder;
DriveHandler *drive_handler;
//AccelerometerHandler *gyro;
TimeOfFlightSensorHandler *ToF;

//LSM303 compass;

// Encoder pin states so PCI can figure which pin changed
volatile int D2_state = LOW;
volatile int D3_state = LOW;
volatile int D4_state = LOW;
volatile int D5_state = LOW;

int state = 0;

int last_ToF_reading = 0;


#define POLE_FINDING

void setup() {
  //Serial.begin(9600);

  // Setting up pin change interrupts to process encoders
  PCICR |= 0b00000100; // enables PCI for post D on atmega328
  PCMSK2 |= 0b00111100; // Trigger on pins D2-D5, the four pins needed for two endcoders

  // Starting wire for I2C sensors
  Wire.begin();
  Wire.setClock(400000);

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
  left_encoder = new EncoderHandler(D2_state, D3_state);
  right_encoder = new EncoderHandler(D4_state, D5_state);

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

  DriveHandler::motorDimensions_t robot_dimensions = {
    WHEEL_RADIUS,
    CIRCLE_DIAMETER,
    COUNT_TO_DISTANCE
  };

  // initialize drive handler
  drive_handler = new DriveHandler(motor_pins, robot_dimensions, left_encoder, right_encoder);

  // initialize LED pins
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  //gyro = new AccelerometerHandler;

  ToF = new TimeOfFlightSensorHandler;

  //compass.init();
  //compass.enableDefault();

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

/*void gyro_reading_interval(AccelerometerHandler *gyro, int interval)
{
  if(millis() - gyro->getLastReading() >= interval)
  {
    gyro->bumpLastReading();
    gyro->takeReading();
    Serial.println(gyro->getOrientation());
  }
}*/

void loop() {

  #ifdef TEST
  drive_handler->drive(255, 1);
  #endif

  #ifdef POLE_FINDING
  if(drive_handler->getRobotAngle() < 180)
  {
    // taking 2 readings just in case one doesn't work
    int reading1 = ToF->getDistance();
    int reading2 = ToF->getDistance();

    //Serial.println(reading1);
    //Serial.println(reading2);

    if(reading1 < 3000 || reading2 < 3000)
    {
      drive_handler->driveUntil(200, 1, min(reading1, reading2) - 25);
      while(1) {}
    }
    else
    {
      drive_handler->turn(MOTOR_FULL_POWER * 0.5, 1);
    }
  }
  else
  {
    drive_handler->stop();
  }
  #endif

  #ifdef GAME_DAY_CODE
  if(state == 0)
  {
    drive_handler->drive(MOTOR_FULL_POWER, 1);
    while(ToF->getDistance() < 4000) {}
    state = 1;
  }
  if(state == 1)
  {
    drive_handler->driveUntil(MOTOR_FULL_POWER, 1, 1200);
    state = 2;
  }
  if(state == 2)
  {
    drive_handler->drive(MOTOR_HALF_POWER, 1);
    while(ToF->getDistance() > 200) {}
    drive_handler->driveUntil(MOTOR_FULL_POWER, 1, 1000);
    drive_handler->turnUntil(255, -1, 90);
    state = 3;
  }
  if(state == 3)
  {
    int reading1 = ToF->getDistance();
    int reading2 = ToF->getDistance();

    //Serial.println(reading1);
    //Serial.println(reading2);

    if(reading1 < 3000 || reading2 < 3000)
    {
      drive_handler->driveUntil(200, 1, min(reading1, reading2) - 25);
      while(1) {}
    }
    else
    {
      drive_handler->turn(MOTOR_FULL_POWER * 0.5, 1);
    }
  }

  #endif

  #ifdef MAIN
  //gyro_reading_interval(gyro, 100);

  if(state == 0) // just started
  {
    drive_handler->driveUntil(MOTOR_FULL_POWER, 1, 1200);
    /*if(gyro->getXZAngle() >= 10)
    {
      state = 1;
    }*/
    state = 1;
  }

  if(state == 1) // going up wall
  {
    drive_handler->driveUntil(MOTOR_FULL_POWER, 1, 1000);
    /*if(gyro->getXZAngle() <= -10)
    {
      state = 2;
    }*/
    state = 2;
  }

  if(state == 2) // going down wall
  {
    drive_handler->driveUntil(MOTOR_HALF_POWER, 1, 1000);
    //if(gyro->getXZAngle() >= -5)
    //{
    drive_handler->driveUntil(127, 1, 100); // driving forward 10cm to make sure robot away from wall
    drive_handler->turnUntil(127, -1, 90); // starting off facing left
    state = 3;
    //}
  }

  if(state == 3) // initial search for pole
  {
    if(drive_handler->getRobotAngle() < 180)
    {
      // taking 2 readings just in case one doesn't work
      int reading1 = ToF->getDistance();
      int reading2 = ToF->getDistance();

      Serial.println(reading1);
      Serial.println(reading2);

      if(reading1 < 2000 || reading2 < 2000)
      {
        drive_handler->driveUntil(200, 1, min(reading1, reading2) - 25);
        state = 5;
      }
      else
      {
        drive_handler->turnUntil(127, 1, 5);
      }
    }
    else
    {
      state = 4;
    }
  }

  if(state == 4)
  {
    // inshallah we don't need this
  }

  if(state == 5)
  {
    drive_handler->stop();
    while(1){}
  }

  #endif
}
