#ifndef PUNCHER_H
#define PUNCHER_H

#include "main.h"

class Puncher {
public:

  // DUAL MOTOR PUNCHER
  /*MotorGroup motor = MotorGroup({
    Motor(4, true, AbstractMotor::gearset::red),
    Motor(5, false, AbstractMotor::gearset::red)
  });*/

  // SINGLE MOTOR PUNCHER
  Motor motor = Motor(4, false, AbstractMotor::gearset::red);

  pros::task_t punchTask = NULL;

  bool idle = true;
  bool wasIdle = true;
  int lastUpdate = 0;

  bool settled = true;

  double targetPos = 0;

  static void runPID(void* self);

  Puncher();
  void punch();
  bool isSettled();
  void waitUntilSettled();
};

#endif
