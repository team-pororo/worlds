#include "twobar.h"

#include "main.h"

const double twoBarPresets[4] = {0, 60, 240, 10};

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
  targetPreset = position;
  targetPosition = twoBarPresets[position] * 5.0 / 3.0;
  motor.moveAbsolute(targetPosition, 100);
}

void TwoBar::avoidPuncherPath() {
  if (targetPreset == 0) {
    moveTo(3);
  }
}

void TwoBar::returnToInitial() {
  if (targetPreset == 3) {
    moveTo(0);
  }
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
