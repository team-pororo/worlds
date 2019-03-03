#ifndef INTAKE_H
#define INTAKE_H

#include "main.h"

enum class IntakeState {
  reverse,
  stopped,
  forward,
  pulseInitial, // to load a ball during a double shot
  pulseWait
};

class Intake {
public:
  Motor motor = Motor(3);
  ControllerButton forwardButton = ControllerButton(ControllerDigital::L1);
  ControllerButton reverseButton = ControllerButton(ControllerDigital::L2);
  IntakeState state = IntakeState::stopped;
  Intake();
  void teleop();
  void update();
  void setState(IntakeState state);
  IntakeState getState();
};

#endif
