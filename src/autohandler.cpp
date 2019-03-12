#include "main.h"
#include "chassis.h"
#include "intake.h"
#include "puncher.h"
#include "autohandler.h"

#include <cstring>

AutoHandler::AutoHandler(Controller& co, Chassis& c, Intake& i, Puncher& p) : controller(co), chassis(c), puncher(p), intake(i) {

}

void AutoHandler::interface(GameState gamestate) {
  switch (gamestate) {
    case (GameState::Initialize): {
      switch (alliance) {
        case (Alliance::Red): {
          controller.setText(0, 0, "RED  ");
        }
        case (Alliance::Blue): {
          controller.setText(0, 0, "BLUE ");
        }
      }

      switch (routine) {
        case (Routine::FrontTile): {
          controller.setText(0, 4, "FRONT TILE");
        }
        case (Routine::BackTile): {
          controller.setText(0, 4, " BACK TILE");
        }
        case (Routine::Skills): {
          controller.setText(0, 4, "    SKILLS");
        }
        case (Routine::Disabled): {
          controller.setText(0, 4, "  DISABLED");
        }
      }

      controller.setText(1, 0, "use triggr btns");

      if (leftAlliance.changedToPressed() || rightAlliance.changedToPressed()) {
        switch (alliance) {

          case (Alliance::Red):
          alliance = Alliance::Blue;
          break;

          case (Alliance::Blue):
          alliance = Alliance::Red;
          break;
        }
      }

      if (leftRoutine.changedToPressed()) {
        switch (routine) {
          case (Routine::FrontTile): {
            routine = Routine::Disabled;
          }
          case (Routine::BackTile): {
            routine = Routine::FrontTile;
          }
          case (Routine::Skills): {
            routine = Routine::BackTile;
          }
          case (Routine::Disabled): {
            routine = Routine::Skills;
          }
        }
      }

      if (rightRoutine.changedToPressed()) {
        switch (routine) {
          case (Routine::FrontTile): {
            routine = Routine::BackTile;
          }
          case (Routine::BackTile): {
            routine = Routine::Skills;
          }
          case (Routine::Skills): {
            routine = Routine::Disabled;
          }
          case (Routine::Disabled): {
            routine = Routine::FrontTile;
          }
        }
      }

    }

    case (GameState::Teleop): {
      int secs_remaining = (105000 - pros::c::millis()) / 1000;
      if (secs_remaining < 0) {
        secs_remaining = 0;
      }
      int mins_remaining = secs_remaining / 60;
      secs_remaining %= 60;
      char time[16] = "       :       ";
      time[6] = '0' + mins_remaining;
      time[8] = '0' + secs_remaining / 10;
      time[9] = '0' + secs_remaining % 10;
      controller.setText(0, 6, time);
      controller.setText(1, 0, "               ");
    }

    case (GameState::Autonomous): {
      switch (alliance) {
        case (Alliance::Red): {
          controller.setText(0, 0, "RED  ");
        }
        case (Alliance::Blue): {
          controller.setText(0, 0, "BLUE ");
        }
      }

      switch (routine) {
        case (Routine::FrontTile): {
          controller.setText(0, 4, "FRONT TILE");
        }
        case (Routine::BackTile): {
          controller.setText(0, 4, " BACK TILE");
        }
        case (Routine::Skills): {
          controller.setText(0, 4, "    SKILLS");
        }
        case (Routine::Disabled): {
          controller.setText(0, 4, "  DISABLED");
        }
      }

      int secs_remaining = (15000 - pros::c::millis()) / 1000;
      if (secs_remaining < 0) {
        secs_remaining = 0;
      }
      char time[16] = "auton run   :  ";
      time[13] = '0' + secs_remaining / 10;
      time[14] = '0' + secs_remaining % 10;
      controller.setText(1, 0, time);
    }

    case (GameState::Disabled): {
      controller.setText(0, 0, "robot  disabled");
    }
  }

  if (controller.getBatteryCapacity() < 20) {
    if (pros::c::battery_get_capacity() < 20) {
      char levels[16] = "BOTH LOW C  R  ";
      levels[9] = '0' + controller.getBatteryCapacity() / 10;
      levels[10] = '0' + controller.getBatteryCapacity() % 10;
      levels[13] = '0' + (int)pros::c::battery_get_capacity() / 10;
      levels[14] = '0' + (int)pros::c::battery_get_capacity() % 10;
      controller.setText(2, 0, levels);
    } else {
      char levels[16] = "CTRL BAT LOW   ";
      levels[14] = '0' + controller.getBatteryCapacity() / 10;
      levels[15] = '0' + controller.getBatteryCapacity() % 10;
      controller.setText(2, 0, levels);
    }
  } else if (pros::c::battery_get_capacity() < 20) {
    char levels[16] = "RBOT BAT LOW   ";
    levels[14] = '0' + (int)pros::c::battery_get_capacity() / 10;
    levels[15] = '0' + (int)pros::c::battery_get_capacity() % 10;
    controller.setText(2, 0, levels);
  }
}

void AutoHandler::autonomous() {
  /*
  // Actually run the autonomous based on the contents of the alliance and routine vars.
  if (routine == Routine::FrontTile || routine == Routine::Skills) {

    // FIRST DOUBLE SHOT - LEFT FLAGPOLE

    intake.state = IntakeState::forward;
    intake.update();

    // DRIVE 42"
    // DRIVE -42"

    intake.state = IntakeState::stopped;
    intake.update();

    // TURN -90deg

    puncher.currentPosition = 1;
    puncher.anglerState = AnglerState::aimHigh;
    while (puncher.anglerState != AnglerState::idle) {
      puncher.update(intake);
    }

    // SECOND DOUBLE SHOT - CENTER FLAGPOLE

    // TURN 45deg
    // DRIVE 24"
    intake.state = IntakeState::forward;
    intake.update();
    // ARM DOWN
    intake.state = IntakeState::stopped;
    intake.update();
    puncher.currentPosition = 1;
    puncher.anglerState = AnglerState::aimHigh;
    while (puncher.anglerState != AnglerState::idle) {
      puncher.update(intake);
    }

    // FLIP CAP

    intake.state = IntakeState::reverse;
    intake.update();
    // DRIVE 24"

    if (routine == Routine::FrontTile) {
      // GET LOW FLAG

      // TURN 15deg
      // DRIVE 18"
      // TURN -45deg
      // DRIVE 12"
      // DRIVE -24"
    } else if (routine == Routine::Skills) {
      // GET ON PLATFORM

      // DRIVE -24"
      // Turn -45deg
      // DRIVE -24"
      // Turn -90deg
      // DRIVE -60"
    }
  } else if (routine == Routine::BackTile) {
    // FIRST DOUBLE SHOT - CENTER FLAGPOLE

    intake.state = IntakeState::forward;
    intake.update();
    // DRIVE 42"
    // TURN -90deg
    intake.state = IntakeState::stopped;
    intake.update();
    // DOUBLE SHOT

    // SECOND DOUBLE SHOT - RIGHT FLAGPOLE

    // Turn 180deg
    // DRIVE 12"

    intake.state = IntakeState::forward;
    intake.update();

    // ARM DOWN

    intake.state = IntakeState::reverse;
    intake.update();

    // DRIVE 24"

    intake.state = IntakeState::forward;
    intake.update();

    // Turn -150deg

    intake.state = IntakeState::stopped;
    intake.update();

    // DOUBLE SHOT
  } else if (routine == Routine::Disabled) {
    // IDK LOL
  }
  */
}
