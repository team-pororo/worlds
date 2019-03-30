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

  ADIButton limsw = ADIButton('A');

  bool idle = true;
  bool wasIdle = true;
  int lastUpdate = 0;

  double targetPos = 0;

  Puncher();
  void punch();
  bool isSettled();
  void waitUntilSettled();
};

#endif
