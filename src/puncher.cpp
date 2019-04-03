#include "puncher.h"

Puncher::Puncher() {
  //motor.setBrakeMode(AbstractMotor::brakeMode::hold);
  motor.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
}

void Puncher::punch() {
  targetPos = motor.getPosition() + 370;
  motor.moveRelative(370, 100);
}

bool Puncher::isSettled() {
  return (abs(targetPos - motor.getPosition()) < 5);
}

void Puncher::waitUntilSettled() {
  while (!isSettled()) {
    pros::Task::delay(20);
  }
}
