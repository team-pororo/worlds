#include "angler.h"

Angler::Angler() {
  motor.setBrakeMode(AbstractMotor::brakeMode::hold);
  calibrate();
}

void Angler::calibrate() {
  motor.tarePosition();
  motor.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
}

void Angler::moveToAngle(double angle) {
  // The following constants were arrived at through regression based
  // on 5 tested data points. It should be "pretty good".

  // This maps the observed angle to the motor shaft angle.
  angle = -1.82e-3 * (angle*angle*angle) + 2.38e-1 * (angle*angle)
                    - 11.9 * angle + 249;

  if (angle < 0) {
    angle = 0;
  }

  if (angle == targetAngle) {
    return; // don't clog up the motor link
  }

  targetAngle = angle;

  motor.moveAbsolute(targetAngle, 100);
}

bool Angler::isSettled() {
  return (abs(motor.getPosition() - targetAngle) < 5);
}

void Angler::waitUntilSettled() {
  while (!isSettled()) {
    pros::Task::delay(10);
  }
}
