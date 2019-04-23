#include "main.h"
#include "watchdog.h"

void Watchdog::start() {
  timeMatchStart = pros::c::millis();
}

void Watchdog::update() {
  batteryCapacity = pros::c::battery_get_capacity();
  controllerCapacity = controller.getBatteryCapacity();

  secsRemaining = (pros::competition::is_autonomous() ? 15 : 105) - (pros::c::millis() - timeMatchStart) / 1000;

  pros::lcd::print(
    5,
    "Time: ms: %05d state: %s remain: :%03d",
    pros::c::millis() % 100000,
    (pros::competition::is_autonomous() ? "AUTO" : "TELE"),
    secsRemaining
  );

  pros::lcd::print(
    0,
    "Battery: Robot: %02d Controller: %02d",
    batteryCapacity,
    controllerCapacity
  );
}

void Watchdog::teleop() {
  update();
}
