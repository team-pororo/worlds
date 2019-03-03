#include "intake.h"
#include "main.h"

Intake::Intake() {
  motor.setBrakeMode(AbstractMotor::brakeMode::hold);
  motor.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
}

void Intake::setState(IntakeState istate) {
  state = istate;
}

IntakeState Intake::getState() {
  return state;
}

void Intake::update() {
  switch (state) {
    case (IntakeState::reverse): {
      pros::lcd::print(1, "Intake: Reverse");
      motor.moveVelocity(-200);
      break;
    }

    case (IntakeState::stopped): {
      pros::lcd::print(1, "Intake: Stopped");
      motor.moveVelocity(0);
      break;
    }

    case (IntakeState::forward): {
      pros::lcd::print(1, "Intake: Forward");
      motor.moveVelocity(200);
      break;
    }

    case (IntakeState::pulseInitial): {
      pros::lcd::print(1, "Intake: Pulsing");
      motor.moveRelative(720, 200);
      state = IntakeState::pulseWait;
      break;
    }

    case (IntakeState::pulseWait): {
      pros::lcd::print(1, "Intake: Pulsing");
      if (abs(motor.getTargetPosition() - motor.getPosition()) < 5) {
        state = IntakeState::stopped;
      }
      break;
    }

  }
}


void Intake::teleop() {
  switch (state) {
    case (IntakeState::reverse): {
      if (forwardButton.changedToPressed()) {
        state = IntakeState::forward;
      } else if (reverseButton.changedToPressed()) {
        state = IntakeState::stopped;
      }
      break;
    }

    case (IntakeState::stopped): {
      if (forwardButton.changedToPressed()) {
        state = IntakeState::forward;
      } else if (reverseButton.changedToPressed()) {
        state = IntakeState::reverse;
      }
      break;
    }

    case (IntakeState::forward): {
      if (forwardButton.changedToPressed()) {
        state = IntakeState::stopped;
      } else if (reverseButton.changedToPressed()) {
        state = IntakeState::reverse;
      }
      break;
    }

    default:
    break;
  }
  update();
}
