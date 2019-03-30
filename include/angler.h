#ifndef ANGLER_H
#define ANGLER_H

#include "main.h"

class Angler {
public:
  Motor motor = Motor(6, true, AbstractMotor::gearset::red);
  Potentiometer pot = Potentiometer('B');

  int targetTicks = 1100;

  double totalError = 0;
  double lastError = 0;

  Angler();
  double getCurrentAngle();
  static void runPID(void* self);
  void moveToAngle(double angle);
  bool isSettled();
  void waitUntilSettled();
};

#endif
