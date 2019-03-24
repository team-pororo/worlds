#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "main.h"

class Watchdog {
public:
  int batteryCapacity;
  int controllerCapacity;
  int timeMatchStart = 0;
  int secsRemaining = 105;

  Controller& controller;

  Watchdog(Controller& c) : controller(c) {};

  void start();
  void update();
  void teleop();
};

#endif
