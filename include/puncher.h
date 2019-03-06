#ifndef PUNCHER_H
#define PUNCHER_H

#include "main.h"
#include "intake.h"

enum class PuncherState {
  pullback,
  ready,
  punching
};

enum class AnglerState {
  aimHigh,
  shootHigh,
  loadSecondBall,
  aimLow,
  shootLow,
  idle,
  flipReset,
  flipUp,
  flipDown
};

// Position 0: Closest field tile
// Position 1: Starting field tile
// Position 2: Platforms-area field tile
// Position 3: Behind Platforms
// Position 4: Constant 45deg ("legacy mode")
const int highFlagAngles[5] = {-0, -30, -60, -90, -30};
const int lowFlagAngles[5] = {-30, -60, -90, -120, -30};

class Puncher {
public:
  Motor motor = Motor(4, true, AbstractMotor::gearset::red);
  Motor angler = Motor(5);
  Vision vision = Vision(6);
  ADIButton limsw = ADIButton('A');

  ControllerButton buttons[5] = {ControllerButton(ControllerDigital::B),
                                 ControllerButton(ControllerDigital::A),
                                 ControllerButton(ControllerDigital::Y),
                                 ControllerButton(ControllerDigital::X),
                                 ControllerButton(ControllerDigital::R1)};

  ControllerButton flip = ControllerButton(ControllerDigital::R2);

  PuncherState state = PuncherState::pullback;
  PuncherState prevState = PuncherState::punching;

  // Angler Double-Shot Positions
  int currentPosition = 0;
  AnglerState anglerState = AnglerState::idle;
  AnglerState prevAnglerState = AnglerState::idle;

  Puncher();
  void tare(bool angleTare);
  void teleop(Intake& intake);
  void update(Intake& intake);
};

#endif
