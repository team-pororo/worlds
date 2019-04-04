#include "puncher.h"

Puncher::Puncher() {
  motor.setBrakeMode(AbstractMotor::brakeMode::coast);
  motor.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
}

// TODO: Add separate task to handle puncher pullback/release

void Puncher::punch() {
  targetPos = motor.getPosition() + 365;
  motor.moveVoltage(0);
  motor.moveRelative(365, 100);
}

bool Puncher::isSettled() {
  return (abs(targetPos - motor.getPosition()) < 5);
}

void Puncher::waitUntilSettled() {
  while (!isSettled()) {
    pros::Task::delay(20);
  }
}
