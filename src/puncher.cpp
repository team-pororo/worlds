#include "puncher.h"

Puncher::Puncher() {
  motor.setBrakeMode(AbstractMotor::brakeMode::hold);
  motor.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
}

// TODO: Add separate task to handle puncher pullback/release

void Puncher::punch() {
  if (state == PuncherState::ready || state == PuncherState::pullback) {
    state = PuncherState::punch;
  } else {
    state = PuncherState::pullbackAndPunch;
  }
  pros::c::task_notify(punchTask);
}

void Puncher::ready() {
  if (state == PuncherState::ready || state == PuncherState::pullback) {
    return;
  }
  state = PuncherState::pullback;
  pros::c::task_notify(punchTask);
}

void Puncher::runPID(void* self_p) {
  Puncher* self = (Puncher*)self_p;
  while (pros::c::task_notify_take(true, TIMEOUT_MAX)) {
    int timeStart = pros::c::millis();

    if (self->state == PuncherState::punch) {
      // Speed-optimized punch - last 15 degrees. Assume puncher is already retracted.

      self->motor.moveRelative(25, 100);
      while (abs(self->motor.getTargetPosition() - self->motor.getPosition()) > 5 && (pros::c::millis() - timeStart > 500)) {
        pros::Task::delay(20);
      }
      self->motor.moveVoltage(0); // let the motor return to initial pos


    } else if (self->state == PuncherState::pullback) {
      // Pull back in advance of a punch to reduce double shot time.

      self->motor.moveRelative(345, 100);
      while (abs(self->motor.getTargetPosition() - self->motor.getPosition()) > 5 && (pros::c::millis() - timeStart > 1000)) {
        pros::Task::delay(20);
      }
      self->motor.moveVelocity(0); // actively keep the motor in position

    } else if (self->state == PuncherState::pullbackAndPunch) {
      // Full 360deg rotation of puncher motor.

      self->motor.moveRelative(370, 100);
      while (abs(self->motor.getTargetPosition() - self->motor.getPosition()) > 5 && (pros::c::millis() - timeStart > 1000)) {
        pros::Task::delay(20);
      }
      self->motor.moveVoltage(0); // let the motor return to initial pos

    }
  }
}

bool Puncher::isSettled() {
  return (state == PuncherState::idle || state == PuncherState::ready);
}

void Puncher::waitUntilSettled() {
  while (!isSettled()) {
    pros::Task::delay(20);
  }
}
