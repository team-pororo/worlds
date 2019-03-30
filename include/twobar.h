#ifndef TWOBAR_H
#define TWOBAR_H

#include "main.h"

const double twoBarPresets[3] = {0, 60, 180};

class TwoBar {
public:
  Motor motor = Motor(5, false, AbstractMotor::gearset::red);

  ControllerButton presetButtons[3] = {
    ControllerButton(ControllerDigital::down),
    ControllerButton(ControllerDigital::right),
    ControllerButton(ControllerDigital::up)
  };

  double targetPosition = 0;

  TwoBar();
  void teleop();
  void moveTo(int preset);
  bool isSettled();
  void waitUntilSettled();
};

#endif
