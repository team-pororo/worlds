#include "chassis.h"

#include <cstring>

Chassis::Chassis(Controller& c) : controller(c) {
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

void Chassis::driveManual() {
  double x, y, v, w, r, l;

  #if DRIVE_ARCADE

  y = controller.getAnalog(ControllerAnalog::rightY);
  x = -controller.getAnalog(ControllerAnalog::leftX);
  v = (127-abs(x)) * (y/127) + y;
  w = (127-abs(y)) * (x/127) + x;
  r = (v + w) / 2;
  l = (v - w) / 2;

  #else

  l = controller.getAnalog(ControllerAnalog::leftY);
  r = controller.getAnalog(ControllerAnalog::rightY);

  #endif

  if (abs(l) < 0.05) {
    l = 0; // 5% deadband
  }
  if (abs(r) < 0.05) {
    r = 0; // 5% deadband
  }

  #if DRIVE_VELOCITY

  r *= 200;
  l *= 200;
  left.moveVelocity(l);
  right.moveVelocity(r);

  #else

  r *= 12000;
  l *= 12000;
  left.moveVoltage(l);
  right.moveVoltage(r);

  #endif
}

void Chassis::update() {
  std::string diagnostics = "Chsis: ";
  if (brakesEngaged) {
    diagnostics += "Brk: On ";
  } else {
    diagnostics += "Brk: No ";
  }

  #if DRIVE_ARCADE
  diagnostics += "Dr: CADE ";
  #else
  diagnostics += "Dr: TANK ";
  #endif

  #if DRIVE_VELOCITY
  diagnostics += "Vel: On ";
  #else
  diagnostics += "Vel: No ";
  #endif

  #if DRIVE_BRAKES
  diagnostics += "DrBr: On ";
  #else
  diagnostics += "DrBr: No ";
  #endif

  pros::lcd::print(2, diagnostics.c_str());
}

void Chassis::teleop() {
  update();
  if (brakeButton.isPressed()) {
    drive.setBrakeMode(AbstractMotor::brakeMode::hold);
    drive.stop(); // hammertime
    brakesEngaged = true;
  } else {
    #if DRIVE_BRAKES
    drive.setBrakeMode(AbstractMotor::brakeMode::hold);
    #else
    drive.setBrakeMode(AbstractMotor::brakeMode::coast);
    #endif
    driveManual();
    brakesEngaged = false;
  }
};
