#ifndef CHASSIS_H
#define CHASSIS_H

#include "main.h"

#define DRIVE_ARCADE false

#define WHEEL_DIAM 4.1_in
#define WHEELBASE_WIDTH 14_in

class Chassis {
public:

  MotorGroup left = MotorGroup({1, 9});
  MotorGroup right = MotorGroup({-2, -10});

  ChassisControllerIntegrated drive = ChassisControllerFactory::create(
    left, right, AbstractMotor::gearset::green, {WHEEL_DIAM, WHEELBASE_WIDTH}
  );
  Controller& controller;
  ControllerButton brakeButton = ControllerButton(ControllerDigital::left);

  Chassis(Controller c);

  void driveStraight(QLength distance);
  void turnInPlace(QAngle angle);
  void driveArc(QLength radius, QAngle angle);

  void teleop();
};

#endif
