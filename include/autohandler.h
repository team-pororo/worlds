#ifndef AUTOHANDLER_H
#define AUTOHANDLER_H

#include "main.h"
#include "chassis.h"
#include "intake.h"
#include "puncher.h"
#include "angler.h"
#include "doubleshots.h"

enum class Alliance {
  Red,
  Blue
};

enum class Routine {
  FrontTile,
  BackTile,
  Skills,
  Disabled
};

enum class GameState {
  Initialize,
  Autonomous,
  Disabled,
  Teleop
};

class AutoHandler {
public:
  Alliance alliance = Alliance::Red;
  Routine routine = Routine::FrontTile;
  Controller& controller;
  Chassis& chassis;
  Intake& intake;
  Puncher& puncher;

  ControllerButton leftAlliance = ControllerButton(ControllerDigital::L1);
  ControllerButton rightAlliance = ControllerButton(ControllerDigital::R1);

  ControllerButton leftRoutine = ControllerButton(ControllerDigital::L2);
  ControllerButton rightRoutine = ControllerButton(ControllerDigital::R2);

  AutoHandler(Controller& co, Chassis& c, Intake& i, Puncher& p);
  void interface(GameState gamestate);
  void autonomous();
};

#endif
