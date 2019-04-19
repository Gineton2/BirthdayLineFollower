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

// "Queue" to track last n measurements of line position
const int qSize = 1;
int sensorQ[qSize];
int qInd = 0;
int total = 0;
int avg = 0;

// Clockwise and counter-clockwise definitions.
// Depending on how you wired your motors, you may need to swap.
#define FORWARD 1
#define REVERSE 0

// Motor definitions to make life easier:
#define MOTOR_R 0
#define MOTOR_L 1

// Pin Assignments //
//Default pins:
#define DIRA 2 // Direction control for motor A
#define PWMA 3  // PWM control (speed) for motor A
#define DIRB 4 // Direction control for motor B
#define PWMB 11 // PWM control (speed) for motor B

#define ON_OFF 13 // on off switch
#define CALIBRATOR 12 //calibration switch

// speed consts
#define MULTIPLIER .75

////Alternate pins:
//#define DIRA 8 // Direction control for motor A
//#define PWMA 9 // PWM control (speed) for motor A
//#define DIRB 7 // Direction control for motor B
//#define PWMB 10 // PWM control (speed) for motor B

void setupQ()
{
  for (int i = 0; i < qSize; i++) {
    // Set whole queue to "line perfectly centered"
    sensorQ[i] = 1300;
  }
}

void setup()
{
  // configure the sensors
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3}, SensorCount);
  //qtr.setEmitterPin(2);

  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);


  setupArdumoto(); // Set all pins as outputs
  setupQ();
}

void loop()
{
  if ( digitalRead(CALIBRATOR) ) {
    calibrateSensors();
  }
  uint16_t position = sensorLoop();

  if (digitalRead(ON_OFF)) {
    if (position < 1500 && position > 1100) {
      driveArdumoto(MOTOR_R, FORWARD, (255*MULTIPLIER));
      driveArdumoto(MOTOR_L, FORWARD, (255*MULTIPLIER));
    } else if (position < 1100) {
      driveArdumoto(MOTOR_R, FORWARD, (255*MULTIPLIER));
      driveArdumoto(MOTOR_L, FORWARD, (0*MULTIPLIER));
    } else {
      driveArdumoto(MOTOR_R, FORWARD, 0*MULTIPLIER);
      driveArdumoto(MOTOR_L, FORWARD, 255*MULTIPLIER);
    }
  } else {
    stopArdumoto(MOTOR_R);
    stopArdumoto(MOTOR_L);
  }

}

// driveArdumoto drives 'motor' in 'dir' direction at 'spd' speed
void driveArdumoto(byte motor, byte dir, byte spd)
{
  if (motor == MOTOR_R)
  {
    digitalWrite(DIRA, dir);
    analogWrite(PWMA, spd);
  }
  else if (motor == MOTOR_L)
  {
    digitalWrite(DIRB, dir);
    analogWrite(PWMB, spd);
  }  
}

void push(int val)
{
  // subtract the last reading:
  total = total - sensorQ[qInd];
  sensorQ[qInd] = val;
  // add the reading to the total:
  total = total + val;
  // advance to the next position in the array:
  qInd = (qInd + 1) % qSize;
  // calculate the average:
  avg = total / qSize;
}

uint16_t sensorLoop()
{
  // read calibrated sensor values and obtain a measure of the line position
  // from 0 to 5000 (for a white line, use readLineWhite() instead)
  uint16_t position = qtr.readLineBlack(sensorValues);

//   print the sensor values as numbers from 0 to 1000, where 0 means maximum
//   reflectance and 1000 means minimum reflectance, followed by the line
//   position
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println(position);
  Serial.println("Switch: " + digitalRead(ON_OFF));

  //delay(250);

  //1300 is about straight
  //800-1000 line to left
  //2000 line to right
  push(position);
  return avg;
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

void calibrateSensors() {
  digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode
  // analogRead() takes about 0.1 ms on an AVR.
  // 0.1 ms per sensor * 4 samples per sensor read (default) * 6 sensors
  // * 10 reads per calibrate() call = ~24 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
//  for (uint16_t i = 0; i < 40; i++)
//  {
//    qtr.calibrate();
//  }
  qtr.calibrate();
  digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration

//  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  //delay(1000);
}
