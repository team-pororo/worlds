#include "intake.h"
#include "main.h"

#include <string.h>

Intake::Intake() {
  motor.setBrakeMode(AbstractMotor::brakeMode::hold);
  motor.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
  vision_low.set_signature(1, &yellow);
  vision_high.set_signature(1, &yellow);
  vision_puncher.set_signature(1, &yellow);
}

void Intake::moveSpeed(int speed) {
  manualControl = true;
  targetSpeed = speed;
  motor.moveVelocity(200*speed);
}

void Intake::moveDistance(double degrees) {
  manualControl = false;

  targetPos += degrees;

  motor.moveAbsolute(targetPos, 200);
}

bool Intake::isSettled() {
  return (abs(motor.getPosition() - targetPos) < 5);
}

void Intake::waitUntilSettled() {
  while (!isSettled()) {
    pros::Task::delay(10);
  }
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

  if (ball.width < 200) {
    return false;
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

void Intake::update() {
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
}

bool Intake::getFull() {
  return (ballPresent(BallPosition::puncher) && (ballPresent(BallPosition::intake) || ballPresent(BallPosition::trajectory)));
}

void Intake::teleop() {
  if (manualControl) {
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

      } else if (newFull != full && newFull) { // If the intake just became full
        moveSpeed(0);
        full = true;

      } else if (getFull() != full) {
        full = false;
      }
    } else if (targetSpeed > 0) {
      if (forwardButton.changedToPressed()) {
        moveSpeed(-200);
      } else if (reverseButton.changedToPressed()) {
        moveSpeed(0);
      }
    }
  }
}
