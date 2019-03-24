#ifndef ANGLER_H
#define ANGLER_H

#include "main.h"

class Angler {
public:
  Motor motor = Motor(6, true, AbstractMotor::gearset::red);
  Potentiometer pot = Potentiometer('B');

  double targetAngle = 0;

  Angler();
  void update();
  void calibrate();
  void moveToAngle(double angle);
  bool isSettled();
  void waitUntilSettled();
};

#endif
