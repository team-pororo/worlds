#ifndef SHOTHANDLER_H
#define SHOTHANDLER_H

#include "main.h"
#include "puncher.h"
#include "angler.h"
#include "twobar.h"
#include "intake.h"


class ShotHandler {
public:
  Puncher& puncher;
  Angler& angler;
  TwoBar& twobar;
  Intake& intake;

  enum class Mode {
    load,
    shoot,
    doubleShoot
  };

  pros::task_t shootTask = NULL;

  bool settled = true;

  Mode mode = Mode::load;

  double targetAngle0 = 45;
  double targetAngle1 = 45;

  ControllerButton zone0 = ControllerButton(ControllerDigital::X);
  ControllerButton zone1 = ControllerButton(ControllerDigital::Y);
  ControllerButton zone2 = ControllerButton(ControllerDigital::A);
  ControllerButton zone3 = ControllerButton(ControllerDigital::B);
  ControllerButton legacy = ControllerButton(ControllerDigital::R1);
  ControllerButton otmr = ControllerButton(ControllerDigital::R2);


  ShotHandler(Puncher& p, Angler& a, Intake& i, TwoBar& t) : puncher(p), angler(a), intake(i), twobar(t) {};

  static void runShoot(void* self);

  void shoot(double angle);
  void doubleShoot(double angle0, double angle1);
  void load();

  void teleop();

  bool isSettled();
  void waitUntilSettled();
};

#endif
