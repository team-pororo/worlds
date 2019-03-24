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

  // Third degree regression seems "good enough" (+/- 2 degrees)

  // This maps the observed angle to the motor shaft angle.

  // INITIAL TEST POINTS:
  // x = desired puncher angle
  // y = actual motor shaft angle
  /*
  x  y
  55 0
  50 20
  47 40
  36 60
  31 80
  27 100
  25 120
  */
  const double a = -8.4825e-3;
  const double b =  1.0498;
  const double c = -45.350;
  const double d =  728.59;

  angle = a * (angle*angle*angle) + b * (angle*angle)
                    + c * angle + d;

  if (angle < 0) {
    angle = 0;
  }

  if (angle > 120) {
    angle = 120;
  }

  if (angle == targetAngle) {
    return; // don't clog up the motor link
  }

  targetAngle = angle;

  motor.moveAbsolute(targetAngle, 100);
}

void Angler::update() {
  pros::lcd::print(4, "Angler: CurrentAngl: %03d TargetAngl: %03d", (int)motor.getPosition(), (int)targetAngle);
}

bool Angler::isSettled() {
  return (abs(motor.getPosition() - targetAngle) < 5);
}

void Angler::waitUntilSettled() {
  while (!isSettled()) {
    pros::Task::delay(10);
  }
}
