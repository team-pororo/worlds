#ifndef DOUBLESHOTS_H
#define DOUBLESHOTS_H

#include "main.h"
#include "puncher.h"
#include "intake.h"
#include "angler.h"

// Position 0: Closest field tile
// Position 1: Starting field tile
// Position 2: Platforms-area field tile
// Position 3: Behind Platforms
// Position 4: Constant 45deg ("legacy mode")
const int highFlagAngles[5] = {60, 50, 45, 30, 45};
const int lowFlagAngles[5]  = {45, 35, 30, 15, 45};

enum class DoubleShotState {
  readyIntakeHigh,
  aimHigh,
  shootHigh,
  intakeBallLow,
  aimLow,
  shootLow,
  idle
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

  DoubleShotState state = DoubleShotState::idle;
  DoubleShotState prev_state = DoubleShotState::shootLow;
  int currentPosition = 0;

  bool intakeReady = true; // to be used with one-touch-made-ready intake
  int otmrMillis = 0; // timeout

  void increment();

  void intakeBall();
  void readyIntake();
  void aim(int position, bool high);
  void shoot();

  bool isSettled();
  void waitUntilSettled();

  void readyIntakeManual();
  void otmrIntake();

  void update();
  void teleop();
};

#endif
