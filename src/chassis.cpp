#include "chassis.h"

void Chassis::teleop(Controller controller) {
  if (brakeButton.isPressed()) {
    pros::lcd::print(0, "Chassis: Braking");
    drive.setBrakeMode(AbstractMotor::brakeMode::hold);
    drive.stop(); // hammertime
  } else {
    #if DRIVE_ARCADE
    pros::lcd::print(0, "Chassis: Driving Arcade");
    #else
    pros::lcd::print(0, "Chassis: Driving Tank");
    #endif

    drive.setBrakeMode(AbstractMotor::brakeMode::coast);
    
    #if DRIVE_ARCADE
    drive.arcade(controller.getAnalog(ControllerAnalog::rightY),
                 controller.getAnalog(ControllerAnalog::leftX));
    #else
    drive.tank(controller.getAnalog(ControllerAnalog::leftY),
               controller.getAnalog(ControllerAnalog::rightY));
    #endif
  }
};
