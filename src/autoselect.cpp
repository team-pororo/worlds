#include "main.h"
#include "autoselect.h"

#include <cstring>

void AutoSelector::update() {
  std::string diagnostics = "Auto: ";

  switch (alliance) {
    case (Alliance::red):
    diagnostics += "Ali: Red ";
    break;

    case (Alliance::blue):
    diagnostics += "Ali: Blu ";
    break;
  }

  switch (tile) {
    case (Tile::front):
    diagnostics += "Tile: Frnt ";
    break;

    case (Tile::back):
    diagnostics += "Tile: Back ";
    break;

    case (Tile::skills):
    diagnostics += "Tile: Skls ";
    break;

    case (Tile::disabled):
    diagnostics += "Tile: None ";
    break;
  }

  if (platform) {
    diagnostics += "Plfm: On ";
  } else {
    diagnostics += "Plfm: No ";
  }

  pros::lcd::print(7, diagnostics.c_str());
}

void AutoSelector::teleop() {
  update();
}

void AutoSelector::runSelector() {
  while (true) {
    update();
    pros::Task::delay(20);
  }
}

void AutoSelector::btn0() {
  switch (alliance) {
    case (Alliance::red) :
    alliance = Alliance::blue;
    break;

    case (Alliance::blue) :
    alliance = Alliance::red;
    break;
  }
}

void AutoSelector::btn1() {
  switch (tile) {
    case (Tile::front) :
    tile = Tile::back;
    break;

    case (Tile::back) :
    tile = Tile::skills;
    break;

    case (Tile::skills) :
    tile = Tile::disabled;
    break;

    case (Tile::disabled) :
    tile = Tile::front;
    break;
  }
}

void AutoSelector::btn2() {
  platform = !platform;
}
