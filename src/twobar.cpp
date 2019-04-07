#include "twobar.h"

#include "main.h"

const double twoBarPresets[3] = {0, 60, 240};

TwoBar::TwoBar() {
  motor.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
  motor.setBrakeMode(AbstractMotor::brakeMode::hold);
}

void TwoBar::drop() {
  int timeStart = pros::c::millis();
  while (pros::c::millis() - timeStart < 5000) {
    if (pot.get() > 330) {
      motor.move(-25);
    } else if (pot.get() < 325) {
      motor.move(25);
    } else {
      motor.moveVelocity(0);
      //break;
    }
  }
  motor.tarePosition();
}

void TwoBar::moveTo(int position) {
  targetPosition = twoBarPresets[position] * 5.0 / 3.0;
  motor.moveAbsolute(targetPosition, 100);
}

bool TwoBar::isSettled() {
  return (abs(targetPosition - motor.getPosition()) < 5);
}

void TwoBar::waitUntilSettled() {
  while (!isSettled()) {
    pros::Task::delay(10);
  }
}

void TwoBar::teleop() {
  for (int i = 0; i < 3; ++i) {
    if (presetButtons[i].changedToPressed()) {
      moveTo(i);
    }
  }
}
