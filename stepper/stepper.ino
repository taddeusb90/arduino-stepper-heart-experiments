#include <Stepper.h>

const int STEPS_PER_REVOLUTION = 2048;
const int CLOCKWISE = 1;
const int COUNTER_CLOCKWISE = 0;
const int STEPS_PER_HALF_REVOLUTION = 1024;
const int STEP = 5;

// states
const int BEGIN = 1;
const int CONTINUE = 2;
const int PAUSE = 3;
const int END = 4;
const int RESET = 5;
const int HOME = 6;


int stepCount = 0;         // number of steps the motor has taken
int motorDirection = CLOCKWISE;
int option = 0;

// initialize the stepper library on pins 8 through 11:
Stepper motor(STEPS_PER_REVOLUTION, 8, 10, 9, 11);

void setup() {
  // initialize the serial port:
  Serial.begin(9600);
  motor.setSpeed(5);
}

void resetPosition() {
//  for (int i=0; i <= stepCount; i++ ) {
    if (motorDirection == CLOCKWISE) {
      motor.step(-stepCount);
    } else if (motorDirection == COUNTER_CLOCKWISE) {
      motor.step(stepCount);
    }
    
    delay(500);
//  }
  stepCount = 0;
}

void moveMotor() {
    if (stepCount >= STEPS_PER_HALF_REVOLUTION && motorDirection == CLOCKWISE) {
      resetPosition();
      motorDirection = COUNTER_CLOCKWISE;
    } else if(stepCount >= STEPS_PER_HALF_REVOLUTION && motorDirection == COUNTER_CLOCKWISE) {
      resetPosition();
      motorDirection = CLOCKWISE;
    }

  if (motorDirection == CLOCKWISE) {
    motor.step(STEP);
  } else if (motorDirection == COUNTER_CLOCKWISE) {
    motor.step(-STEP);
  }
  
  stepCount += STEP;
  delay(500);
}

void fullRevolution() {
  motor.step(STEPS_PER_REVOLUTION);
  delay(5000);
}

void loop() {
  delay(3);
  if (Serial.available()) {
     option = Serial.parseInt();  //gets one byte from serial buffer
  }

  switch (option) {
    case BEGIN: 
      resetPosition();
      moveMotor();
      Serial.println(BEGIN);
      break;
    case CONTINUE:
      moveMotor();
      Serial.println(CONTINUE);
      break;      
    case PAUSE:
      Serial.println(PAUSE);
      break;
    case END:
      resetPosition();
      Serial.println(END);
      break;
    case RESET:
      resetPosition();
      Serial.println(RESET);
      break;
    case HOME:
      fullRevolution();
      break;
    default: 
      break;
  }
  option = 0;  
}
