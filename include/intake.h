#ifndef INTAKE_H
#define INTAKE_H

#include "main.h"

enum class BallPosition {
  intake,
  trajectory,
  puncher
};

class Intake {
public:
  Motor motor = Motor(3, false, AbstractMotor::gearset::green);

  Vision vision_low = Vision(11);
  Vision vision_high = Vision(12);
  Vision vision_puncher = Vision(13);

  pros::vision_signature_s_t yellow =
          pros::Vision::signature_from_utility(1,579,1399,989,-4303,-3339,-3821,3,0);

  ControllerButton forwardButton = ControllerButton(ControllerDigital::L1);
  ControllerButton reverseButton = ControllerButton(ControllerDigital::L2);

  Intake();

  bool manualControl = true;
  int targetSpeed = 0;
  double targetPos = 0;

  void moveSpeed(int speed);
  void moveDistance(double degrees);

  bool isSettled();
  void waitUntilSettled();

  bool ballPresent(BallPosition position);

  void teleop();
};

#endif
