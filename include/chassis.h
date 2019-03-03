#ifndef CHASSIS_H
#define CHASSIS_H

#include "main.h"

#define DRIVE_ARCADE false

class Chassis {
public:
  ChassisControllerIntegrated drive = ChassisControllerFactory::create(
    {1, 9}, {-2, -10}, AbstractMotor::gearset::green, {4.1_in, 13.75_in}
  );
  ControllerButton brakeButton = ControllerButton(ControllerDigital::left);
  void teleop(Controller s);
};

#endif
