#ifndef ANGLER_H
#define ANGLER_H

#include "main.h"

class Angler {
public:
  Motor motor = Motor(6, true, AbstractMotor::gearset::red);
  Potentiometer pot = Potentiometer('B');

  int targetTicks = 1100;

  int timeStart = pros::c::millis();

  double totalError = 0;
  double lastError = 0;

  Angler();
  static void runPID(void* self);
  void moveToAngle(bool high, int pos);
  bool isSettled();
  void waitUntilSettled();
};

#endif
