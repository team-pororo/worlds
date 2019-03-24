#include "doubleshots.h"
#include "puncher.h"
#include "angler.h"
#include "intake.h"

#include <cstring>

void DoubleShotHandler::increment() {
  loopIndex++;
  if (loopIndex >= currentLoop->size()) {
    loopIndex = currentLoop->size() - 1; // Don't go past the end
  }
  firstRun = true;
  lastUpdateMillis = pros::c::millis();
}

/*void DoubleShotHandler::detectBall() {
  if (firstRun) {
    firstRun = false;
  }
  if (currentLoop->at(loopIndex).flag == Flag::high) {
    if (!intake.ballPresent(BallPosition::puncher)) {
      currentLoop = &idleLoop;
      loopIndex = 0;
    } else {
      increment(); // Get ready to aim!
    }

  } else {
    if (!intake.ballPresent(BallPosition::intake) && !intake.ballPresent(BallPosition::trajectory) && !intake.ballPresent(BallPosition::puncher)) {
      currentLoop = &idleLoop;
      loopIndex = 0;
    } else {
      increment(); // Get ready to aim!
    }
  }
}*/

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
  // Preliminary aim - start aiming *while* getting the ball ready.
  if (firstRun) {
    firstRun = false;
  }
  if (currentLoop->at(loopIndex).flag == Flag::high) {
    angler.moveToAngle(highFlagAngles[position]);
  } else {
    angler.moveToAngle(midFlagAngles[position]);
  }

  increment();

}

void DoubleShotHandler::waitForAim() {
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

      if (intake.ballPresent(BallPosition::puncher)) {
        if (intake.ballPresent(BallPosition::intake) || intake.ballPresent(BallPosition::trajectory)) {
          currentLoop = &doubleShotLoop; // Two balls loaded - double-shot it!
          //pros::lcd::print(0, "Two balls loaded, double-shot");
        } else {
          currentLoop = &highShotLoop; // Only ball in the puncher, no ball in intake
          //pros::lcd::print(0, "No intake ball, high shot");
        }
      } else {
        if (intake.ballPresent(BallPosition::intake) || intake.ballPresent(BallPosition::trajectory)) {
          currentLoop = &midShotLoop; // Only ball in the intake, no ball in puncher
          //pros::lcd::print(0, "No puncher ball, middle shot");
        } else {
          currentLoop = &idleLoop; // No balls
          //pros::lcd::print(0, "No balls present, no shot");
        }
      }
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
  std::string diagnostics = "DSHandler:";
  // DIAGNOSTIC INFO
  if (currentLoop == &doubleShotLoop) {
    diagnostics += " Lp: Doubl";
  } else if (currentLoop == &singleShotLoop) {
    diagnostics += " Lp: Lgacy";
  } else if (currentLoop == &idleLoop) {
    diagnostics += " Lp: Idle ";
  } else if (currentLoop == &highShotLoop) {
    diagnostics += " Lp: HiFlg";
  } else if (currentLoop == &midShotLoop) {
    diagnostics += " Lp: MdFlg";
  } else {
    diagnostics += " Lp: Error";
  }

  switch (currentLoop->at(loopIndex).state) {
    case (DSState::intakeBall):
    diagnostics += " St: Intak";
    break;

    case (DSState::clearIntake):
    diagnostics += " St: ClrIn";
    break;

    case (DSState::aim):
    diagnostics += " St: Aimin";
    break;

    case (DSState::waitForAim):
    diagnostics += " St: WtAim";
    break;

    case (DSState::shoot):
    diagnostics += " St: Shoot";
    break;

    case (DSState::idle):
    diagnostics += " St: Idle ";
    break;

    default:
    diagnostics += " St: Error";
  }

  switch (currentLoop->at(loopIndex).flag) {
    case (Flag::high):
    diagnostics += " Flag: Hi ";
    break;

    case (Flag::mid):
    diagnostics += " Flag: Mid";
    break;

    case (Flag::low):
    diagnostics += " Flag: Low";
    break;
  }

  pros::lcd::print(6, diagnostics.c_str());

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

    case (DSState::waitForAim):
    waitForAim();
    break;

    case (DSState::shoot):
    shoot();
    break;

    case (DSState::idle):
    idle();
    break;
  }

  if (pros::c::millis() - 5000 > lastUpdateMillis) {
    if (!isSettled()) {
      increment(); // Timeout
    }
  }

  puncher.update();
  angler.update();
  intake.update();
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
}
