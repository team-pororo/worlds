#include "chassis.h"

Chassis::Chassis(Controller c) : controller(c) {
  drive.setBrakeMode(AbstractMotor::brakeMode::hold);
  left.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
  right.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
}

void Chassis::driveStraight(QLength distance) {
  drive.moveDistance(distance);
}

void Chassis::turnInPlace(QAngle angle) {
  drive.turnAngle(angle);
}

void Chassis::driveArc(QLength radius, QAngle angle) {
  bool leftward = (radius < 0_m);
  radius = QLength(abs(radius.convert(meter)));

  QLength radius_l, radius_r;

  if (leftward) {
    radius_l = radius - (WHEELBASE_WIDTH / 2);
    radius_r = radius + (WHEELBASE_WIDTH / 2);
  } else {
    radius_l = radius + (WHEELBASE_WIDTH / 2);
    radius_r = radius - (WHEELBASE_WIDTH / 2);
  }

  QLength distance_l, distance_r;

  distance_l = (angle / 360_deg) * radius_l * (2*M_PI);
  distance_r = (angle / 360_deg) * radius_r * (2*M_PI);

  QAngularSpeed rpm_l, rpm_r;

  if (leftward) {
    rpm_l = 200_rpm * (distance_l / distance_r);
    rpm_r = 200_rpm;
  } else {
    rpm_l = 200_rpm;
    rpm_r = 200_rpm * (distance_r / distance_l);
  }

  left.moveRelative((distance_l / (WHEEL_DIAM * M_PI)).getValue() * 360, rpm_l.convert(rpm));
  right.moveRelative((distance_r / (WHEEL_DIAM * M_PI)).getValue() * 360, rpm_r.convert(rpm));
}

void Chassis::teleop() {
  if (brakeButton.isPressed()) {
    pros::lcd::print(0, "Chassis: Braking");

    drive.stop(); // hammertime
  } else {
    #if DRIVE_ARCADE
    pros::lcd::print(0, "Chassis: Driving Arcade");
    #else
    pros::lcd::print(0, "Chassis: Driving Tank");
    #endif

    #if DRIVE_ARCADE
    drive.arcade(controller.getAnalog(ControllerAnalog::rightY),
                 controller.getAnalog(ControllerAnalog::leftX));
    #else
    drive.tank(controller.getAnalog(ControllerAnalog::leftY),
               controller.getAnalog(ControllerAnalog::rightY));
    #endif
  }
};
