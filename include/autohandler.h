#ifndef AUTOHANDLER_H
#define AUTOHANDLER_H

#include "main.h"
#include "chassis.h"
#include "intake.h"
#include "puncher.h"

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

  AutoHandler(Controller& co, Chassis& c, Intake& i, Puncher& p);
  void interface(GameState gamestate);
  void autonomous();
};

#endif
