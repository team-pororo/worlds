#ifndef AUTOSELECT_H
#define AUTOSELECT_H

#include "main.h"

enum class Alliance {
  red,
  blue
};

enum class Tile {
  front,
  back,
  skills,
  disabled
};

class AutoSelector {
public:
  Alliance alliance = Alliance::red;
  Tile tile = Tile::disabled;
  bool platform = true;

  void update();
  void teleop();
  void runSelector();

  void btn0();
  void btn1();
  void btn2();
};

#endif
