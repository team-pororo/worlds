#include "angler.h"

const double kP = 0.2;
const double kI = 0.0002;
const double kD = 0.001;

Angler::Angler() {
  motor.setBrakeMode(AbstractMotor::brakeMode::hold);
  motor.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
  lastError = pot.get() - targetTicks;
}

double Angler::getCurrentAngle() {
  double reading = pot.get();
  // Formula contructed on the following data:
  // Angle = 55 deg -> Pot ticks = 1000
  // Angle = 30 deg -> Pot ticks = 1370
  return (-5/64 * reading + 4385/32);
}

void Angler::moveToAngle(double angle) {
  targetTicks = (-64/5 * angle + 1754);
  totalError = 0;
  timeStart = pros::c::millis();
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


    pros::Task::delay(20);
  }
}

bool Angler::isSettled() {
  return (abs(pot.get() - targetTicks) < 10 || (pros::c::millis() - timeStart > 1000));
}

void Angler::waitUntilSettled() {
  while (!isSettled()) {
    pros::Task::delay(10);
  }
}
