    /* Ardumoto Example Sketch
  by: Jim Lindblom
  date: November 8, 2013
  license: Public domain. Please use, reuse, and modify this 
  sketch!

  Adapted to v20 hardware by: Marshall Taylor
  date: March 31, 2017

  Three useful functions are defined:
    setupArdumoto() -- Setup the Ardumoto Shield pins
    driveArdumoto([motor], [direction], [speed]) -- Drive [motor] 
      (0 for A, 1 for B) in [direction] (0 or 1) at a [speed]
      between 0 and 255. It will spin until told to stop.
    stopArdumoto([motor]) -- Stop driving [motor] (0 or 1).

  setupArdumoto() is called in the setup().
  The loop() demonstrates use of the motor driving functions.
*/

#include <QTRSensors.h>

QTRSensors qtr;

// Sensor init
const uint8_t SensorCount = 4;
uint16_t sensorValues[SensorCount];

// Clockwise and counter-clockwise definitions.
// Depending on how you wired your motors, you may need to swap.
#define FORWARD  1
#define REVERSE 0

// Motor definitions to make life easier:
#define MOTOR_A 0
#define MOTOR_B 1

// Pin Assignments //
//Default pins:
#define DIRA 2 // Direction control for motor A
#define PWMA 3  // PWM control (speed) for motor A
#define DIRB 4 // Direction control for motor B
#define PWMB 11 // PWM control (speed) for motor B

#define ONOFF 2 // on off switch

////Alternate pins:
//#define DIRA 8 // Direction control for motor A
//#define PWMA 9 // PWM control (speed) for motor A
//#define DIRB 7 // Direction control for motor B
//#define PWMB 10 // PWM control (speed) for motor B


void setup()
{
    // configure the sensors
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3}, SensorCount);
  qtr.setEmitterPin(2);

  //delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode

  pinMode(ONOFF, INPUT);
  
//  // analogRead() takes about 0.1 ms on an AVR.
//  // 0.1 ms per sensor * 4 samples per sensor read (default) * 6 sensors
//  // * 10 reads per calibrate() call = ~24 ms per calibrate() call.
//  // Call calibrate() 400 times to make calibration take about 10 seconds.
//  for (uint16_t i = 0; i < 400; i++)
//  {
//    qtr.calibrate();
//  }
//  digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration
//
//  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
//  for (uint8_t i = 0; i < SensorCount; i++)
//  {
//    Serial.print(qtr.calibrationOn.minimum[i]);
//    Serial.print(' ');
//  }
//  Serial.println();
//
//  // print the calibration maximum values measured when emitters were on
//  for (uint8_t i = 0; i < SensorCount; i++)
//  {
//    Serial.print(qtr.calibrationOn.maximum[i]);
//    Serial.print(' ');
//  }
//  Serial.println();
//  Serial.println();
//  delay(1000);
  setupArdumoto(); // Set all pins as outputs
}

void loop()
{
  
  if ( digitalRead(ONOFF) == 1 ) {
//      uint16_t position = sensorLoop();
      driveArdumoto(MOTOR_A, FORWARD, 255);
      driveArdumoto(MOTOR_B, FORWARD, 255);
      Serial.println(digitalRead(ONOFF));

  } else {
    Serial.println(digitalRead(ONOFF));
    stopArdumoto(MOTOR_A);
    stopArdumoto(MOTOR_B);
  }
//  if (position < 2500) {
//    driveArdumoto(MOTOR_A, FORWARD, 255);
//    driveArdumoto(MOTOR_B, FORWARD, 255);
//  } else {
//    driveArdumoto(MOTOR_B, FORWARD, 255);
//    driveArdumoto(MOTOR_A, FORWARD, 120);
//  }
//  // Drive motor A (and only motor A) at various speeds, then stop.
//  driveArdumoto(MOTOR_A, REVERSE, 255); // Set motor A to REVERSE at max
//  delay(1000);  // Motor A will spin as set for 1 second
//  driveArdumoto(MOTOR_A, FORWARD, 127);  // Set motor A to FORWARD at half
//  delay(1000);  // Motor A will keep trucking for 1 second 
//  stopArdumoto(MOTOR_A);  // STOP motor A 
//
//  // Drive motor B (and only motor B) at various speeds, then stop.
//  driveArdumoto(MOTOR_B, REVERSE, 255); // Set motor B to REVERSE at max
//  delay(1000);  // Motor B will spin as set for 1 second
//  driveArdumoto(MOTOR_B, FORWARD, 127);  // Set motor B to FORWARD at half
//  delay(1000);  // Motor B will keep trucking for 1 second
//  stopArdumoto(MOTOR_B);  // STOP motor B 
//
//  // Drive both
//  driveArdumoto(MOTOR_A, FORWARD, 255);  // Motor A at max speed.
//  driveArdumoto(MOTOR_B, FORWARD, 255);  // Motor B at max speed.
//  delay(1000);  // Drive forward for a second
//  // Now go backwards at half that speed:
//  driveArdumoto(MOTOR_A, REVERSE, 127);  // Motor A at max speed.
//  driveArdumoto(MOTOR_B, REVERSE, 127);  // Motor B at max speed.
//  delay(1000);  // Drive forward for a second
//
//  // Now spin in place!
//  driveArdumoto(MOTOR_A, FORWARD, 255);  // Motor A at max speed.
//  driveArdumoto(MOTOR_B, REVERSE, 255);  // Motor B at max speed.
//  delay(2000);  // Drive forward for a second
//  stopArdumoto(MOTOR_A);  // STOP motor A 
//  stopArdumoto(MOTOR_B);  // STOP motor B 


}

// driveArdumoto drives 'motor' in 'dir' direction at 'spd' speed
void driveArdumoto(byte motor, byte dir, byte spd)
{
  if (motor == MOTOR_A)
  {
    digitalWrite(DIRA, dir);
    analogWrite(PWMA, spd);
  }
  else if (motor == MOTOR_B)
  {
    digitalWrite(DIRB, dir);
    analogWrite(PWMB, spd);
  }  
}

uint16_t sensorLoop()
{
  // read calibrated sensor values and obtain a measure of the line position
  // from 0 to 5000 (for a white line, use readLineWhite() instead)
  uint16_t position = qtr.readLineBlack(sensorValues);

  // print the sensor values as numbers from 0 to 1000, where 0 means maximum
  // reflectance and 1000 means minimum reflectance, followed by the line
  // position
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println(position);

  delay(250);

  return position;
}
// stopArdumoto makes a motor stop
void stopArdumoto(byte motor)
{
  driveArdumoto(motor, 0, 0);
}

// setupArdumoto initialize all pins
void setupArdumoto()
{
  // All pins should be setup as outputs:
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);

  // Initialize all pins as low:
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}
