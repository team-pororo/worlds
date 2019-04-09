#ifndef TWOBAR_H
#define TWOBAR_H

#include "main.h"

class TwoBar {
public:
  Motor motor = Motor(5, true, AbstractMotor::gearset::red);
  Potentiometer pot = Potentiometer('A');

  ControllerButton presetButtons[3] = {
    ControllerButton(ControllerDigital::down),
    ControllerButton(ControllerDigital::right),
    ControllerButton(ControllerDigital::up)
  };

  int targetPreset = 0;
  double targetPosition = 0;

  TwoBar();
  void drop();
  void teleop();
  void moveTo(int preset);
  void avoidPuncherPath();
  void returnToInitial();
  bool isSettled();
  void waitUntilSettled();
};

#endif
