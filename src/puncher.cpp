#include "puncher.h"

Puncher::Puncher() {
  motor.setBrakeMode(AbstractMotor::brakeMode::hold);
  tare();
}

void Puncher::tare() {
  motor.tarePosition();
  motor.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
}

void Puncher::update() {
  // Update Puncher
  switch (state) {
    case (PuncherState::pullback): {
      pros::lcd::print(5, "Puncher: Pulling Back");

      if (prevState != state) {
        prevState = state;
        motor.moveAbsolute(135, 100);
        lastUpdate = pros::c::millis();
      }

      if (abs(motor.getPosition() - 135) < 5 || pros::c::millis() - lastUpdate > 2000) {
        state = PuncherState::ready;
      }
      break;
    }

    case (PuncherState::ready): {
      pros::lcd::print(5, "Puncher: Ready");
      if (prevState != state) {
        prevState = state;
        lastUpdate = pros::c::millis();
      }
      break;
    }

    case (PuncherState::punching): {
      pros::lcd::print(5, "Puncher: Punching");
      if (prevState != state) {
        prevState = state;
        lastUpdate = pros::c::millis();
      }

      if (limsw.isPressed() || pros::c::millis() - lastUpdate > 2000) {
        state = PuncherState::pullback;
        tare();

      } else {
        motor.moveVelocity(100);
      }

      break;
    }
  }

}
