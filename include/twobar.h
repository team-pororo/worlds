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

  double targetPosition = 0;

  TwoBar();
  void drop();
  void teleop();
  void moveTo(int preset);
  bool isSettled();
  void waitUntilSettled();
};

#endif
