#include "main.h"
#include "controllerLCD.h"
#include "watchdog.h"
#include "autoselect.h"

#include <string>

void ControllerLCD::init() {
  // Splash screen will put the controller in 3-line text mode
  // This task takes 500ms to execute - use SPARINGLY!
  pros::Task::delay(100);
  controller.clear();
  /*pros::Task::delay(100);
  controller.setText(0, 0, "Made  with  <3");
  pros::Task::delay(100);
  controller.setText(0, 0, "by  wesleychal");
  pros::Task::delay(100);
  controller.setText(0, 0, "4393S @MSSM,ME");
  pros::Task::delay(100);
  //controller.clear();*/
  pros::Task::delay(100);
  lastUpdate = pros::c::millis();
}

void ControllerLCD::update() {
  //init();
  if (pros::c::millis() - lastUpdate < 200) {
    return; // Don't flood controller with updates
  }

  std::string line0 = "";
  switch (autoSelector.alliance) {
    case (Alliance::red):
    line0 += "RED  ";
    break;

    case (Alliance::blue):
    line0 += "BLUE ";
    break;
  }

  switch (autoSelector.tile) {
    case (Tile::front):
    line0 += "FRNT ";
    break;

    case (Tile::back):
    line0 += "BACK ";
    break;

    case (Tile::skills):
    line0 += "SKLS ";
    break;

    case (Tile::disabled):
    line0 += "NONE ";
    break;
  }

  if (autoSelector.platform) {
    line0 += "PLFM";
  } else {
    line0 += "NOPL";
  }


  if (pros::competition::get_status() == 0b100) { // opcontrol
    controller.setText(2, 0, "Time: " + std::to_string(watchdog.secsRemaining) + " Batt: " + std::to_string(watchdog.controllerCapacity));
  } else {
    controller.setText(2, 0, line0);
  }
}

void ControllerLCD::teleop() {
  update();
}
