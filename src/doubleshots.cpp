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
  loopIndex++;
  if (loopIndex >= currentLoop->size()) {
    loopIndex = currentLoop->size() - 1; // Don't go past the end
  }
  firstRun = true;
}

void DoubleShotHandler::clearIntake() {
  if (firstRun) {
    firstRun = false;
  }
  if (intake.ballPresent(BallPosition::trajectory)) {
    intake.moveSpeed(200);
  } else {
    intake.moveSpeed(0);
    increment();
  }
}

void DoubleShotHandler::intakeBall() {
  if (firstRun) {
    firstRun = false;
  }
  if (!intake.ballPresent(BallPosition::puncher)) {
    intake.moveSpeed(-200);
  } else {
    intake.moveSpeed(0);
    increment();
  }
}

void DoubleShotHandler::aim() {
  if (firstRun) {
    firstRun = false;
  }
  if (currentLoop->at(loopIndex).flag == Flag::high) {
    angler.moveToAngle(highFlagAngles[position]);
  } else {
    angler.moveToAngle(midFlagAngles[position]);
  }
  if (angler.isSettled()) {
    increment();
  }
}

void DoubleShotHandler::shoot() {
  if (firstRun) {
    firstRun = false;
    puncher.state = PuncherState::punching;
  }
  puncher.update();
  if (puncher.state == PuncherState::ready) {
    increment();
  }
}

void DoubleShotHandler::idle() {
  currentLoop = &idleLoop;
  loopIndex = 0;
  firstRun = false;

  for (int i = 0; i < 4; ++i) {
    if (zoneButtons[i].isPressed()) {
      position = i;
      currentLoop = &doubleShotLoop;
      loopIndex = 0;
    }
  }

  if (legacyButton.isPressed()) {
    position = 4;
    currentLoop = &singleShotLoop;
    loopIndex = 0;
  }

  if (otmrButton.isPressed()) {
    position = -1;
    currentLoop = &otmrLoop;
    loopIndex = 0;
  }
}

void DoubleShotHandler::update() {
  // DIAGNOSTIC INFO
  if (currentLoop == &doubleShotLoop) {
    pros::lcd::print(4, "Doing Double Shot");
  } else if (currentLoop == &singleShotLoop) {
    pros::lcd::print(4, "Doing Single Shot");
  } else if (currentLoop == &idleLoop) {
    pros::lcd::print(4, "Idle");
  } else {
    pros::lcd::print(4, "Unknown State");
  }

  switch (currentLoop->at(loopIndex).state) {
    case (DSState::intakeBall):
    pros::lcd::print(5, "Intaking Ball");
    break;

    case (DSState::clearIntake):
    pros::lcd::print(5, "Clearing Intake");
    break;

    case (DSState::aim):
    pros::lcd::print(5, "Aiming");
    break;

    case (DSState::shoot):
    pros::lcd::print(5, "Shooting");
    break;

    case (DSState::idle):
    pros::lcd::print(5, "Idle");
    break;

    default:
    pros::lcd::print(5, "Unknown State");
  }

  switch (currentLoop->at(loopIndex).flag) {
    case (Flag::high):
    pros::lcd::print(6, "High Flag");
    break;

    case (Flag::mid):
    pros::lcd::print(6, "Middle Flag");
    break;

    case (Flag::low):
    pros::lcd::print(6, "Low Flag");
    break;
  }

  switch (currentLoop->at(loopIndex).state) {
    case (DSState::intakeBall):
    intakeBall();
    break;

    case (DSState::clearIntake):
    clearIntake();
    break;

    case (DSState::aim):
    aim();
    break;

    case (DSState::shoot):
    shoot();
    break;

    case (DSState::idle):
    idle();
    break;
  }
}

bool DoubleShotHandler::isSettled() {
  return (currentLoop == &idleLoop);
}

void DoubleShotHandler::waitUntilSettled() {
  while (!isSettled()) {
    update();
    pros::Task::delay(10);
  }
}

void DoubleShotHandler::teleop() {
  update();
  puncher.update();
}
