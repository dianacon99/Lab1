/********
Default E80 Code
Team:
    Bowen
    Diana
    Ilona
    Weston
Current Author:
    Wilson Ives (wives@g.hmc.edu) '20 (contributed in 2018)
Previous Contributors:
    Christopher McElroy (cmcelroy@g.hmc.edu) '19 (contributed in 2017)  
    Josephine Wong (jowong@hmc.edu) '18 (contributed in 2016)
    Apoorva Sharma (asharma@hmc.edu) '17 (contributed in 2016)                    
*/

/* Libraries */

// general
#include <Arduino.h>
#include <Wire.h>
#include <Pinouts.h>

// E80-specific
#include <SensorIMU.h>
#include <MotorDriver.h>
#include <Logger.h>
#include <Printer.h>

// Motor Test:1 
// Tank Test: 0
#define TEST 1

/* Global Variables */

// period in ms of logger and printer
#define LOOP_PERIOD 100

// Motors
MotorDriver motorDriver;

// IMU
SensorIMU imu;

// Logger
Logger logger;
bool keepLogging = true;

// Printer
Printer printer;

// loop start recorder
int loopStartTime;

void motorTest();
void tankTest();

void setup() {
  printer.init();

  /* Initialize the Logger */
  logger.include(&imu);
  logger.include(&motorDriver);
  logger.init();

  /* Initialise the sensors */
  imu.init();

  /* Initialize motor pins */
  motorDriver.init();

  /* Keep track of time */
  printer.printMessage("Starting main loop",10);
  loopStartTime = millis();
}

#if TEST
void motorTest(){
  // Program to test the 3 motors

  int currentTime = millis() - loopStartTime;

  if (currentTime < 2000) {
    // off for 2 sec
    motorDriver.drive(0,0,0);

  } else if ( currentTime < 4000 ){
    // vertical motor
    motorDriver.drive(255, 0, 0);

  } else if ( currentTime < 6000 ){
    // off
    motorDriver.drive(0, 0, 0);

  } else if ( currentTime < 8000 ){
    // port motor
    motorDriver.drive(0, 255, 0);

  } else if ( currentTime < 10000 ){
    // off
    motorDriver.drive(0, 0, 0);

  } else if ( currentTime < 12000 ){
    // starboard motor
    motorDriver.drive(0, 0, 255);

  } else {
    // off
    motorDriver.drive(0, 0 , 0);
  }
}
#else
void tankTest() {
  // Program to be run for the tank test

  int waitTime = 15000;
  int forwardTime1 = 3000 + waitTime;
  int raiseTime1 = 2000 + forwardTime1;
  int forwardTime2 = 3000 + raiseTime1;
  int raiseTime2 = 2000 + forwardTime2;
  int forwardTime3 = 3000 + raiseTime2;

  int currentTime = millis() - loopStartTime;
  if (currentTime < waitTime) {
      // wait with motors off
      motorDriver.drive(0,0,0);

    } else if ( currentTime < forwardTime1 ){
      // move forward
      motorDriver.drive(255, 0, 0);

    } else if ( currentTime < raiseTime1 ){
      // rise
      motorDriver.drive(0, 0, 0);

    } else if ( currentTime < forwardTime2 ){
      // move forward
      motorDriver.drive(0, 255, 0);

    } else if ( currentTime < raiseTime2 ){
      // rise
      motorDriver.drive(0, 0, 0);

    } else if ( currentTime < forwardTime3 ){
      // move forward
      motorDriver.drive(0, 0, 255);

    } else {
      // off
      motorDriver.drive(0, 0 , 0);
    }

}
#endif

void loop() {

  int currentTime = millis() - loopStartTime;
  
  ///////////  Don't change code above here! ////////////////////
  // write code here to make the robot fire its motors in the sequence specified in the lab manual 
  // the currentTime variable contains the number of ms since the robot was turned on 
  // The motorDriver.drive function takes in 3 inputs arguments motorA_power, motorB_power, motorC_power: 
  //       void motorDriver.drive(int motorA_power,int motorB_power,int motorC_power); 
  // the value of motorX_power can range from -255 to 255, and sets the PWM applied to the motor 
  // The following example will turn on motor B for four seconds between seconds 4 and 8 

  // Example Code
  // if (currentTime > 4000 && currentTime <8000) {
  //   motorDriver.drive(0,120,0);
  // } else {
  //   motorDriver.drive(0,0,0);
  // }

  if (TEST) {
    motorTest();
  } else {
    tankTest();
  }

  // DONT CHANGE CODE BELOW THIS LINE 
  // --------------------------------------------------------------------------
  
  if ( currentTime-printer.lastExecutionTime > LOOP_PERIOD ) {
    printer.lastExecutionTime = currentTime;
    printer.printValue(0,imu.printAccels());
    printer.printValue(1,imu.printRollPitchHeading());
    printer.printValue(2,motorDriver.printState());
    printer.printToSerial();  // To stop printing, just comment this line out
  }

  if ( currentTime-imu.lastExecutionTime > LOOP_PERIOD ) {
    imu.lastExecutionTime = currentTime;
    imu.read(); // this is a sequence of blocking I2C read calls
  }

  if ( currentTime-logger.lastExecutionTime > LOOP_PERIOD && logger.keepLogging) {
    logger.lastExecutionTime = currentTime;
    logger.log();
  }

}
