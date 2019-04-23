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
  int targetTicks = 300;

  int timeStart = pros::c::millis();

  double totalError = 0;
  double lastError = 0;


  TwoBar();
  void teleop();
  void moveTo(int preset);
  static void runPID(void* self);
  void avoidPuncherPath();
  void returnToInitial();
  bool isSafeToShoot();
  void waitUntilSafeToShoot();
  bool isSettled();
  void waitUntilSettled();
};

#endif
