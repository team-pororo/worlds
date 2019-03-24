#ifndef CONTROLLERLCD_H
#define CONTROLLERLCD_H

#include "main.h"
#include "autoselect.h"
#include "watchdog.h"

class ControllerLCD {
public:
  Controller& controller;
  AutoSelector& autoSelector;
  Watchdog& watchdog;
  int lastUpdate = 0;
  int frame = 0;

  ControllerLCD(Controller& c, AutoSelector& a, Watchdog& w) : controller(c), autoSelector(a), watchdog(w) {};

  void init();
  void update();
  void teleop();

};

#endif
