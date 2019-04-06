#include "twobar.h"

#include "main.h"

const double twoBarPresets[3] = {5, 60, 205};

TwoBar::TwoBar() {
  motor.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
  motor.setBrakeMode(AbstractMotor::brakeMode::hold);
  lastError = pot.get() - targetTicks;
}

const double kPP = 0;//.0001;
const double kP = 0.15;
const double kI = 0.0001;
const double kD = 0.00005;//.37;

void TwoBar::moveTo(int position) {
  targetTicks = (17 * twoBarPresets[position] + 348); // placeholder
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

    double speed = error * error * kPP;
    if (error < 0) {
      speed = -speed;
    }
    speed += error * kP;
    speed += self->totalError * kI;
    speed += (error - self->lastError) * kD;
    pros::lcd::print(6, "TwoBar Speed: %03d Error: %03d", (int)speed, (int)error);

    //self->motor.moveVelocity(-speed);

    self->lastError = error;


    pros::Task::delay(20);
  }
}

bool TwoBar::isSettled() {
  //return (abs(pot.get() - targetTicks) < 20 || (pros::c::millis() - timeStart > 5000));
  return true;
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
