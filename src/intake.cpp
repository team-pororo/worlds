#include "intake.h"
#include "main.h"

#include <string.h>

Intake::Intake() {
  motor.setBrakeMode(AbstractMotor::brakeMode::hold);
  motor.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
  vision_low.set_signature(1, &yellow_low);
  vision_high.set_signature(1, &yellow_high);
  vision_puncher.set_signature(1, &yellow_puncher);
}

void Intake::moveSpeed(int speed) {
  targetSpeed = speed;
  motor.moveVelocity(200*speed);
}


bool Intake::ballPresentRaw(BallPosition position) {
  pros::vision_object_s_t ball;
  switch (position) {
    case (BallPosition::intake):
    ball = vision_low.get_by_sig(0, 1);
    break;

    case (BallPosition::trajectory):
    ball = vision_high.get_by_sig(0, 1);
    break;

    case (BallPosition::puncher):
    ball = vision_puncher.get_by_sig(0, 1);
    break;
  }

  if (ball.signature != 1) {
    return false;
  }

  if (position != BallPosition::trajectory) {
    // Most sensors are mounted sideways
    if (ball.height < 100) {
      return false;
    }
  } else {
    if (ball.width < 200) {
      return false;
    }
  }

  return true;
}

bool Intake::ballPresent(BallPosition position) {
  switch (position) {
    case (BallPosition::intake):
    return (filter_low.filter(ballPresentRaw(position)) > 0.5);
    break;

    case (BallPosition::trajectory):
    return (filter_high.filter(ballPresentRaw(position)) > 0.5);
    break;

    case (BallPosition::puncher):
    return (filter_puncher.filter(ballPresentRaw(position)) > 0.5);
    break;
  }
}

/*void Intake::update() {
  pros::lcd::print(
    3,
    "Intake: Low: (%01d,%01d) Hi: (%01d,%01d) Pnch: (%01d,%01d)",
    ballPresentRaw(BallPosition::intake),
    ballPresent(BallPosition::intake),
    ballPresentRaw(BallPosition::trajectory),
    ballPresent(BallPosition::trajectory),
    ballPresentRaw(BallPosition::puncher),
    ballPresent(BallPosition::puncher)
  );
}*/

bool Intake::getFull() {
  return (ballPresent(BallPosition::puncher) && (ballPresent(BallPosition::intake) || ballPresent(BallPosition::trajectory)));
}

void Intake::runVision(void* self_p) {
  Intake* self = (Intake*)self_p;
  while (true) {
    pros::lcd::print(
      2,
      "Intk: L(%01d,%01d) H(%01d,%01d) P(%01d,%01d) ST:(%01d)",
      self->ballPresentRaw(BallPosition::intake),
      self->ballPresent(BallPosition::intake),
      self->ballPresentRaw(BallPosition::trajectory),
      self->ballPresent(BallPosition::trajectory),
      self->ballPresentRaw(BallPosition::puncher),
      self->ballPresent(BallPosition::puncher),
      self->isSettled()
    );
    pros::Task::delay(20);
  }
}

void Intake::runFunctions(void* self_p) {
  Intake* self = (Intake*)self_p;
  while (pros::c::task_notify_take(true, TIMEOUT_MAX))  {
    int timeStart = pros::c::millis();
    switch (self->action) {
      case (IntakeAction::load): {

        self->moveSpeed(-200);
        while (!self->ballPresent(BallPosition::puncher) && (pros::c::millis() - timeStart < 1000)) {
          pros::Task::delay(20);
        }
        self->moveSpeed(0);

      }
      break;
      case (IntakeAction::clear): {

        self->moveSpeed(200);
        while (self->ballPresent(BallPosition::trajectory) && (pros::c::millis() - timeStart < 1000)) {
          pros::Task::delay(20);
        }
        self->moveSpeed(0);

      }
      break;
      case (IntakeAction::loadAndClear): {

        self->moveSpeed(-200);
        while (!self->ballPresent(BallPosition::puncher) && (pros::c::millis() - timeStart < 1000)) {
          pros::Task::delay(20);
        }
        self->moveSpeed(0);

        self->moveSpeed(200);
        while (self->ballPresent(BallPosition::trajectory) && (pros::c::millis() - timeStart < 1000)) {
          pros::Task::delay(20);
        }
        self->moveSpeed(0);

      }
    }
    self->settled = true;
  }
}

void Intake::load() {
  action = IntakeAction::load;
  settled = false;
  pros::c::task_notify(functionTask);
}

void Intake::clear() {
  action = IntakeAction::clear;
  settled = false;
  pros::c::task_notify(functionTask);
}

void Intake::ready() {
  action = IntakeAction::loadAndClear;
  settled = false;
  pros::c::task_notify(functionTask);
}

bool Intake::isSettled() {
  return settled;
}

void Intake::waitUntilSettled() {
  while (!settled) {
    pros::Task::delay(20);
  }
}

void Intake::teleop() {
  if (targetSpeed == 0) {
    if (forwardButton.changedToPressed()) {
      moveSpeed(-200);
    } else if (reverseButton.changedToPressed()) {
      moveSpeed(200);
    }
  } else if (targetSpeed < 0) {
    bool newFull = getFull();

    if (forwardButton.changedToPressed()) {
      moveSpeed(0);
    } else if (reverseButton.changedToPressed()) {
      moveSpeed(200);

    }/* else if (newFull != full && newFull) { // If the intake just became full
      moveSpeed(0);
      full = true;

    } else if (getFull() != full) {
      full = false;
    }*/
  } else if (targetSpeed > 0) {
    if (forwardButton.changedToPressed()) {
      moveSpeed(-200);
    } else if (reverseButton.changedToPressed()) {
      moveSpeed(0);
    }
  }
}
