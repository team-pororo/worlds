#ifndef INTAKE_H
#define INTAKE_H

#include "main.h"
#include <vector>

enum class BallPosition {
  intake,
  trajectory,
  puncher
};

enum class IntakeAction {
  load,
  clear,
  loadAndClear
};

class Intake {
public:
  Motor motor = Motor(3, false, AbstractMotor::gearset::green);

  Vision vision_low = Vision(11);
  Vision vision_high = Vision(12);
  Vision vision_puncher = Vision(13);

  MedianFilter<7> filter_low;
  MedianFilter<7> filter_high;
  MedianFilter<7> filter_puncher;

  pros::task_t functionTask = NULL;
  IntakeAction action = IntakeAction::load;
  bool settled = true;

  pros::vision_signature_s_t yellow =
          pros::Vision::signature_from_utility(1,579,1399,989,-4303,-3339,-3821,3,0);

  ControllerButton forwardButton = ControllerButton(ControllerDigital::L1);
  ControllerButton reverseButton = ControllerButton(ControllerDigital::L2);

  Intake();

  int targetSpeed = 0;

  bool full = false;

  void moveSpeed(int speed);

  bool ballPresentRaw(BallPosition position);
  bool ballPresent(BallPosition position);
  bool getFull();

  void load();
  void clear();
  void ready();

  bool isSettled();
  void waitUntilSettled();

  static void runVision(void* self);
  static void runFunctions(void* self);
  void teleop();
};

#endif
