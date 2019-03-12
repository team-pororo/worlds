#ifndef PUNCHER_H
#define PUNCHER_H

#include "main.h"

enum class PuncherState {
  pullback,
  ready,
  punching
};

class Puncher {
public:
  MotorGroup motor = MotorGroup({
    Motor(4, true, AbstractMotor::gearset::red),
    Motor(5, false, AbstractMotor::gearset::red)
  });
  ADIButton limsw = ADIButton('A');

  PuncherState state = PuncherState::pullback;
  PuncherState prevState = PuncherState::punching;
  int lastUpdate = 0;

  Puncher();
  void tare();
  void update();
};

#endif
