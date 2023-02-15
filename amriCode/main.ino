#include <Wire.h>

#include "MPU9250.h";

// install new library from https://www.hackster.io/hibit/getting-real-time-position-using-mpu9250-2ec046
// another good one https://registry.platformio.org/libraries/bolderflight/Bolder%20Flight%20Systems%20MPU9250


// GYRO ____________________________________________________|
#define MPU9250_IMU_ADDRESS 0x68

#define MAGNETIC_DECLINATION 1.63 // To be defined by user
#define INTERVAL_MS_PRINT 1000

MPU9250 mpu;

unsigned long lastPrintMillis = 0;



// SIMULTANEOUS THREADS ______________________________________|
unsigned long previousMillis_1 = 0; //store time for first event
unsigned long previousMillis_2 = 0; //store time for second event

const long interval_1 = 1000; //interval for first event
const long interval_2 = 2000; //interval for second event


// ULTRASONIC __________________________________________________|
const int trigPin = 9;
const int echoPin = 10;

long duration;
int distance;





const int LED = 2;

void setup() {
  // put your setup code here, to run once:

  // ULTRASONIC __________________________________________|
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Starting...");

  MPU9250Setting setting;

  // Sample rate must be at least 2x DLPF rate
  setting.accel_fs_sel = ACCEL_FS_SEL::A16G;
  setting.gyro_fs_sel = GYRO_FS_SEL::G1000DPS;
  setting.mag_output_bits = MAG_OUTPUT_BITS::M16BITS;
  setting.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_250HZ;
  setting.gyro_fchoice = 0x03;
  setting.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_20HZ;
  setting.accel_fchoice = 0x01;
  setting.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_45HZ;

  mpu.setup(MPU9250_IMU_ADDRESS, setting);

  mpu.setMagneticDeclination(MAGNETIC_DECLINATION);
  mpu.selectFilter(QuatFilterSel::MADGWICK);
  mpu.setFilterIterations(15);

  Serial.println("Calibration will start in 5sec.");
  Serial.println("Please leave the device still on the flat plane.");
  delay(5000);

  Serial.println("Calibrating...");
  mpu.calibrateAccelGyro();

  Serial.println("Magnetometer calibration will start in 5sec.");
  Serial.println("Please Wave device in a figure eight until done.");
  delay(5000);

  Serial.println("Calibrating...");
  mpu.calibrateMag();

  Serial.println("Ready!");



}

void simulThreads () {
  unsigned long currentMillis = millis();

  //conditional that checks whether 1 second has passed since last event
  if (currentMillis - previousMillis_1 >= interval_1) {
      previousMillis_1 = millis();
      //execute a piece of code, every *1 second*
      digitalWrite(LED, HIGH); //turn on an LED
      ultrasonicReading();
      
  }

  //conditional that checks whether 2 seconds have passed since last event
  if (currentMillis - previousMillis_2 >= interval_2) {
      previousMillis_2 = millis();
      digitalWrite(LED, LOW); //program is unpaused, and the LED is turned off
      readingGyro();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  simulThreads();
  // delay(1000);
    

  
}

void readingGyro () {
  
  unsigned long currentMillis = millis();

  if (mpu.update() && currentMillis - lastPrintMillis > INTERVAL_MS_PRINT) {
    Serial.print("TEMP:\t");
    Serial.print(mpu.getTemperature(), 2);
    Serial.print("\xC2\xB0"); //Print degree symbol
    Serial.print("C");
    Serial.println();

    Serial.print("Pitch:\t");
    Serial.print(mpu.getPitch());
    Serial.print("\xC2\xB0"); //Print degree symbol
    Serial.println();

    Serial.print("Roll:\t");
    Serial.print(mpu.getRoll());
    Serial.print("\xC2\xB0"); //Print degree symbol
    Serial.println();

    Serial.print("Yaw:\t");
    Serial.print(mpu.getYaw());
    Serial.print("\xC2\xB0"); //Print degree symbol
    Serial.println();

    Serial.println();

    lastPrintMillis = currentMillis;
  }
}


void ultrasonicReading () {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;

  Serial.print("Distance: ");
  Serial.println(distance);
}
