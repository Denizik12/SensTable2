#include <Servo.h>

Servo servoLeft;
Servo servoRight;

void setup() {
  servoLeft.attach(10);
  servoRight.attach(9);
}

void loop() {
  int value = 1;
  checkPos(value);
}

void servoClose() {
  int possitionLeftServo = 180;
  int possitionRightServo = 0;
  servoLeft.write(possitionLeftServo);
  servoRight.write(possitionRightServo);
}

void servoOpen() {
  int possitionLeftServo = 90;
  int possitionRightServo = 90;
  servoLeft.write(possitionLeftServo);
  servoRight.write(possitionRightServo);
}

void checkPos(int input) {
  if (input == 1) {
    servoClose();
  } else {
    servoOpen();
  }
}
