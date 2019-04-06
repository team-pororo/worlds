#include "puncher.h"

Puncher::Puncher() {
  motor.setBrakeMode(AbstractMotor::brakeMode::coast);
  motor.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
}

// TODO: Add separate task to handle puncher pullback/release

void Puncher::punch() {
  settled = false;
  pros::c::task_notify(punchTask);
}

void Puncher::runPID(void* self_p) {
  Puncher* self = (Puncher*)self_p;
  while (pros::c::task_notify_take(true, TIMEOUT_MAX)) {
  //while (true) {
    //self->settled = false;

    //self->targetPos = self->motor.getPosition() + 365;
    self->motor.moveRelative(370, 100);
    while (abs(self->motor.getTargetPosition() - self->motor.getPosition()) > 5) {
      pros::Task::delay(20);
    }
    self->motor.moveVoltage(0); // let the motor return to initial pos

    self->settled = true;
  }
}

bool Puncher::isSettled() {
  return settled;
}

void Puncher::waitUntilSettled() {
  while (!isSettled()) {
    pros::Task::delay(20);
  }
}
