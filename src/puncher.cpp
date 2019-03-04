#include "puncher.h"
#include "intake.h"

Puncher::Puncher() {
  motor.setBrakeMode(AbstractMotor::brakeMode::hold);
  angler.setBrakeMode(AbstractMotor::brakeMode::hold);
  tare();
}

void Puncher::tare() {
  motor.tarePosition();
  motor.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
  angler.tarePosition();
  angler.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
}

void Puncher::update(Intake& intake) {
  // Update Puncher
  switch (state) {
    case (PuncherState::pullback): {
      pros::lcd::print(2, "Puncher: Pulling Back");

      if (prevState != state) {
        prevState = state;
        motor.move_relative(135, 100);
      }

      if (abs(motor.getPosition() - motor.getTargetPosition()) < 5) {
        state = PuncherState::ready;
      }
      break;
    }

    case (PuncherState::ready): {
      pros::lcd::print(2, "Puncher: Ready");
      if (prevState != state) {
        prevState = state;
      }
      break;
    }

    case (PuncherState::punching): {
      pros::lcd::print(2, "Puncher: Punching");
      if (prevState != state) {
        prevState = state;
      }

      if (limsw.changedToPressed()) {
        state = PuncherState::pullback;
        tare();

      } else {
        motor.moveVelocity(100);
      }

      break;
    }
  }

  // Update Angler
  switch (anglerState) {
    case (AnglerState::aimHigh): {
      pros::lcd::print(3, "Angler: Aiming High");
      intake.state = IntakeState::stopped;
      if (prevAnglerState != anglerState) {
        angler.moveAbsolute(highFlagAngles[currentPosition], 200);
        prevAnglerState = anglerState;
      }

      if (abs(angler.getTargetPosition() - angler.getPosition()) < 2) {
        anglerState = AnglerState::shootHigh;
      }
      break;
    }

    case (AnglerState::shootHigh): {
      pros::lcd::print(3, "Angler: Shooting High");
      if (prevAnglerState != anglerState) {
        state = PuncherState::punching;
        prevAnglerState = anglerState;
      }

      if (state == PuncherState::pullback) {
        anglerState = AnglerState::loadSecondBall;
      }
      break;
    }

    case (AnglerState::loadSecondBall): {
      pros::lcd::print(3, "Angler: Loading Second Ball");
      if (prevAnglerState != anglerState) {
        intake.setState(IntakeState::pulseInitial);
        prevAnglerState = anglerState;
      }

      if (intake.getState() == IntakeState::stopped && state == PuncherState::ready) {
        anglerState = AnglerState::aimLow;
      }
      break;
    }

    case (AnglerState::aimLow): {
      pros::lcd::print(3, "Angler: Aiming Low");

      if (prevAnglerState != anglerState) {
        angler.moveAbsolute(lowFlagAngles[currentPosition], 200);
        prevAnglerState = anglerState;
      }

      if (abs(angler.getTargetPosition() - angler.getPosition()) < 2) {
        anglerState = AnglerState::shootLow;
      }

      break;
    }

    case (AnglerState::shootLow): {
      pros::lcd::print(3, "Angler: Shooting Low");

      if (prevAnglerState != anglerState) {
        state = PuncherState::punching;
        prevAnglerState = anglerState;
      }

      if (state == PuncherState::pullback) {
        anglerState = AnglerState::idle;
      }

      break;
    }

    case (AnglerState::idle): {
      pros::lcd::print(3, "Angler: Idle");

      if (prevAnglerState != anglerState) {
        prevAnglerState = anglerState;
      }

      break;
    }

    case (AnglerState::flipReset): {
      pros::lcd::print(3, "Angler: Resetting");

      if (prevAnglerState != anglerState) {
        prevAnglerState = anglerState;
        angler.moveAbsolute(0, 200);
      }

      if (abs(angler.getTargetPosition() - angler.getPosition()) < 5) {
        anglerState = AnglerState::flipUp;
      }

      break;
    }

    case (AnglerState::flipUp): {
      pros::lcd::print(3, "Angler: Flipping Up");

      if (prevAnglerState != anglerState) {
        prevAnglerState = anglerState;
        angler.moveAbsolute(-60, 200);
      }

      if (abs(angler.getTargetPosition() - angler.getPosition()) < 5) {
        anglerState = AnglerState::flipDown;
      }

      break;
    }

    case (AnglerState::flipDown): {
      pros::lcd::print(3, "Angler: Flipping Down");

      if (prevAnglerState != anglerState) {
        prevAnglerState = anglerState;
        angler.moveAbsolute(0, 200);
      }

      if (abs(angler.getTargetPosition() - angler.getPosition()) < 5) {
        anglerState = AnglerState::idle;
      }

      break;
    }
  }
}

void Puncher::teleop(Intake& intake) {
  if (anglerState == AnglerState::idle) {
    for (int i = 0; i < 5; ++i) {
      if (buttons[i].isPressed()) {
        currentPosition = i;
        if (i == 4) { // Position 4 is special, it's a one-shot 45deg pos
          anglerState = AnglerState::aimLow;
        } else {
          anglerState = AnglerState::aimHigh;
        }
        break;
      }
    }
    if (flip.isPressed()) {
      anglerState = AnglerState::flipReset;
    }
  }
  update(intake);
}
