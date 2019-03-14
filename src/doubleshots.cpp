#include "doubleshots.h"
#include "puncher.h"
#include "angler.h"
#include "intake.h"

void DoubleShotHandler::readyIntakeManual() {
  // run the One-Touch-Made-Ready intake routine, set flag if ready.

  if (pros::c::millis() - otmrMillis > 2000) {
    intakeReady = true;
    return;
  }


  if (intake.ballPresent(BallPosition::puncher)) {
    if (intake.ballPresent(BallPosition::trajectory)) {
      intake.moveSpeed(200); // Get the second ball out of the way
      intakeReady = false;
    } else {
      intake.moveSpeed(0); // Ready to fire
      intakeReady = true;
    }
  } else {
    intake.moveSpeed(-200); // Load the initial ball
    intakeReady = false;
  }
}

// One-touch-made-ready intake
// If set to true, tap button to run macro
// If set to false, hold button to run macro
#define OTMR_WAIT_FOR_COMPLETION true

void DoubleShotHandler::otmrIntake() {
  if (otmrButton.isPressed()) {
    otmrMillis = pros::c::millis();
  }

#if OTMR_WAIT_FOR_COMPLETION
  if (otmrButton.isPressed() || !intakeReady) {

#else
  if (otmrButton.isPressed()) {

#endif
    readyIntakeManual();
  }
}

void DoubleShotHandler::increment() {
  // Hackish but it works.
  prev_state = state;
  state = static_cast<DoubleShotState>(static_cast<int>(state) + 1);
}

void DoubleShotHandler::readyIntake() {
  if (prev_state != state) {
    prev_state = state;
  }
  if (intake.ballPresent(BallPosition::trajectory)) {
    intake.moveSpeed(200);
  } else {
    intake.moveSpeed(0);
    increment();
  }
}

void DoubleShotHandler::intakeBall() {
  if (prev_state != state) {
    prev_state = state;
  }
  if (!intake.ballPresent(BallPosition::puncher)) {
    intake.moveSpeed(-200);
  } else {
    intake.moveSpeed(0);
    increment();
  }
}

void DoubleShotHandler::aim(int position, bool high) {
  if (prev_state != state) {
    prev_state = state;
  }
  if (high) {
    angler.moveToAngle(highFlagAngles[position]);
  } else {
    angler.moveToAngle(lowFlagAngles[position]);
  }
  if (angler.isSettled()) {
    increment();
  }
}

void DoubleShotHandler::shoot() {
  if (prev_state != state) {
    prev_state = state;
    puncher.state = PuncherState::punching;
  }
  puncher.update();
  if (puncher.state == PuncherState::ready) {
    increment();
  }
}

void DoubleShotHandler::update() {
  switch (state) {
    case (DoubleShotState::idle):
    pros::lcd::print(3, "DoubleShot: Idle");
    break;

    case (DoubleShotState::readyIntakeHigh):
    pros::lcd::print(3, "DoubleShot: Clearing Intake to Shoot High");
    readyIntake();
    break;

    case (DoubleShotState::aimHigh):
    pros::lcd::print(3, "DoubleShot: Aiming at High Flag");
    aim(currentPosition, true);
    break;

    case (DoubleShotState::shootHigh):
    pros::lcd::print(3, "DoubleShot: Shooting at High Flag");
    shoot();
    break;

    case (DoubleShotState::intakeBallLow):
    pros::lcd::print(3, "DoubleShot: Intaking Ball to Shoot Low");
    intakeBall();
    break;

    case (DoubleShotState::aimLow):
    pros::lcd::print(3, "DoubleShot: Aiming at Low Flag");
    aim(currentPosition, false);
    break;

    case (DoubleShotState::shootLow):
    pros::lcd::print(3, "DoubleShot: Shooting at Low Flag");
    shoot();
    break;
  }
}

bool DoubleShotHandler::isSettled() {
  return (state == DoubleShotState::idle);
}

void DoubleShotHandler::waitUntilSettled() {
  while (state != DoubleShotState::idle) {
    update();
    pros::Task::delay(10);
  }
}

void DoubleShotHandler::teleop() {
  update();
  puncher.update();
  if (state == DoubleShotState::idle) {
    otmrIntake();
    if (intakeReady) {
      for (int i = 0; i < 4; ++i) {
        if (zoneButtons[i].isPressed()) {
          currentPosition = i;
          state = DoubleShotState::readyIntakeHigh;
        }
      }
      if (legacyButton.isPressed()) {
        currentPosition = 4;
        state = DoubleShotState::aimLow;
      }
    }
  }
}
