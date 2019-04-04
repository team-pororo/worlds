#include "twobar.h"

#include "main.h"

const double twoBarPresets[3] = {0, 60, 200};

TwoBar::TwoBar() {
  motor.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
  motor.setBrakeMode(AbstractMotor::brakeMode::hold);
  lastError = pot.get() - targetTicks;
}

const double kP = 0.2;
const double kI = 0.0002;
const double kD = 0.001;

void TwoBar::moveTo(int position) {
  targetTicks = (2 * twoBarPresets[position] + 1000); // placeholder
  totalError = 0;
  timeStart = pros::c::millis();
}

void TwoBar::runPID(void* self_p) {
  TwoBar* self = (TwoBar*)self_p;
  for (;;) {

    //pros::lcd::print(4, "Angler: CurrentAngl: %04d TargetAngl: %04d", (int)self->pot.get(), self->targetTicks);

    double reading = self->pot.get();
    double error = reading - self->targetTicks;
    self->totalError += error;

    double speed = error * kP;
    speed += self->totalError * kI;
    speed += (error - self->lastError) * kD;
    //pros::lcd::print(4, "Angler Speed: %03d Error: %03d", (int)speed, (int)error);

    self->motor.moveVelocity(-speed);

    self->lastError = error;


    pros::Task::delay(20);
  }
}

bool TwoBar::isSettled() {
  return (abs(pot.get() - targetTicks) < 10 || (pros::c::millis() - timeStart > 5000));
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
