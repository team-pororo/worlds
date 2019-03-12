#include "intake.h"
#include "main.h"

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

bool Intake::ballPresent(BallPosition position) {
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

  if (ball.width < 400) {
    return false;
  }

  return true;
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
      if (forwardButton.changedToPressed()) {
        moveSpeed(0);
      } else if (reverseButton.changedToPressed()) {
        moveSpeed(200);
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
