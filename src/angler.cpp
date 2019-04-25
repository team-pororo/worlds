#include "angler.h"

const double kP = 0.4;
const double kI = 0.004;
const double kD = 0.0;

// Motor shaft degrees for each preset - FIND THESE EXPERIMENTALLY
// Zone A Mid Flag is too high, but the angler can't go any lower!
const double highAngles[5] = {53, 40, 40, 45, 40};
const double lowAngles[5] = {35, 28, 25, 30, 40};

Angler::Angler() {
  motor.setBrakeMode(AbstractMotor::brakeMode::brake);
  motor.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
}

void Angler::runPID(void* self_p) {
  Angler* self = (Angler*)self_p;
  for (;;) {

    pros::lcd::print(4, "Angler: CurrentAngl: %04d TargetAngl: %04d", (int)self->pot.get(), self->targetTicks);

    double reading = self->pot.get();
    double error = reading - self->targetTicks;
    self->totalError += error;

    double speed = error * kP;
    speed += self->totalError * kI;
    speed += (error - self->lastError) * kD;
    pros::lcd::print(4, "Angler Speed: %03d Error: %03d", (int)speed, (int)error);

    self->motor.moveVelocity(-speed);

    self->lastError = error;


    pros::Task::delay(5);
  }
}

void Angler::moveToAngle(bool high, int position) {
  double angle;
  if (high) {
    angle = highAngles[position];
  } else {
    angle = lowAngles[position];
  }
  targetTicks = (-16.4 * angle + 1880);
  if (targetTicks > 1370) {
    targetTicks = 1370;
  } else if (targetTicks < 1000) {
    targetTicks = 1000;
  }
  totalError = 0;
  timeStart = pros::c::millis();
}

bool Angler::isSettled() {
  return (abs(pot.get() - targetTicks) < 20 || (pros::c::millis() - timeStart > 1000));
}

void Angler::waitUntilSettled() {
  while (!isSettled()) {
    pros::Task::delay(10);
  }
}
