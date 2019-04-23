#ifndef PUNCHER_H
#define PUNCHER_H

#include "main.h"

enum class PuncherState {
  idle,
  pullback,
  ready,
  punch,
  pullbackAndPunch
};

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

  PuncherState state = PuncherState::idle;

  double targetPos = 0;

  static void runPID(void* self);

  Puncher();
  void ready();
  void punch();
  void update();
  bool isSettled();
  void waitUntilSettled();
};

#endif
