#ifndef DOUBLESHOTS_H
#define DOUBLESHOTS_H

#include "main.h"
#include "puncher.h"
#include "intake.h"
#include "angler.h"

#include <vector>

// Position 0: Closest field tile
// Position 1: Starting field tile
// Position 2: Platforms-area field tile
// Position 3: Behind Platforms
// Position 4: Constant 45deg ("legacy mode")
const int highFlagAngles[5] = {60, 50, 45, 30, 45};
const int midFlagAngles[5]  = {45, 35, 30, 15, 45};

enum class DSState {
  intakeBall,
  clearIntake,
  aim,
  shoot,
  idle
};

enum class Flag {
  high,
  mid,
  low
};

class dss_t {
public:
  DSState state;
  Flag flag;
};

const std::vector<dss_t> doubleShotLoop = {
  {DSState::clearIntake, Flag::high},
  {DSState::aim, Flag::high},
  {DSState::shoot, Flag::high},
  {DSState::intakeBall, Flag::mid},
  {DSState::clearIntake, Flag::mid},
  {DSState::aim, Flag::low},
  {DSState::shoot, Flag::low},
  {DSState::idle, Flag::low}
};

const std::vector<dss_t> singleShotLoop = {
  /*{DSState::intakeBall, Flag::mid},*/
  {DSState::clearIntake, Flag::mid},
  {DSState::aim, Flag::mid},
  {DSState::shoot, Flag::mid},
  {DSState::idle, Flag::mid}
};

const std::vector<dss_t> otmrLoop = {
  {DSState::intakeBall, Flag::mid},
  {DSState::clearIntake, Flag::mid},
  {DSState::idle, Flag::mid}
};

const std::vector<dss_t> idleLoop = {
  {DSState::idle, Flag::mid}
};

class DoubleShotHandler {
public:
  Puncher puncher;
  Angler angler;
  Intake intake;

  DoubleShotHandler(Puncher& p, Angler& a, Intake& i) : puncher(p), angler(a), intake(i) {};

  ControllerButton zoneButtons[4] = {
    ControllerButton(ControllerDigital::B),
    ControllerButton(ControllerDigital::A),
    ControllerButton(ControllerDigital::Y),
    ControllerButton(ControllerDigital::X)
  };

  ControllerButton legacyButton = ControllerButton(ControllerDigital::R1);

  ControllerButton otmrButton = ControllerButton(ControllerDigital::R2);

  const std::vector<dss_t>* currentLoop = &idleLoop;
  int loopIndex = currentLoop->size() - 1;

  int position = 0; // 0 is closest, 3 is farthest, 4 is 45deg singleshot

  bool firstRun; // Flag set whenever a new state is selected

  bool intakeReady = true; // to be used with one-touch-made-ready intake
  int otmrMillis = 0; // timeout

  void increment();

  void intakeBall();
  void clearIntake();
  void aim();
  void shoot();
  void idle(); // wait for controller press

  bool isSettled();
  void waitUntilSettled();

  void readyIntakeManual();
  void otmrIntake();

  void update();
  void teleop();
};

#endif
