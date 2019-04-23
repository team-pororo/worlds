#include "main.h"
#include "chassis.h"
#include "intake.h"
#include "twobar.h"
#include "autoselect.h"
#include "shothandler.h"

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

extern AutoSelector autoSelector;
extern Chassis chassis;
extern Intake intake;
extern TwoBar twobar;
extern ShotHandler shotHandler;

// Red Alliance Side of Field
// North
// Ai B1 C1 Flags
// A2 B2 C2 Flat Caps
// A3 B3 C3 Tilted Caps
// A4 B4 C4 Platforms
// A5 B5 C5 Tilted Caps
// A6 B6 C6 Expansion Zone, Flat Caps

void autonomous() {
  switch (autoSelector.tile) {
    case (Tile::front): { // 3hf + 2lf + 2c + plfm = 13pts
      // START:A3 Facing EAST
      // Intake Forward
      // Drive forward to C3
      chassis.driveStraight(48_in);
      // Intake Load Function
      // Reverse to A3 (a bit east)
      chassis.driveStraight(-48_in);
      // Turn LEFT to face NORTH
      if (autoSelector.alliance == Alliance::red) {
        chassis.turnAngle(-90_deg);
      } else {
        chassis.turnAngle(90_deg);
      }
      // Double Shot - 1 Position


      // Drive to A2
      chassis.driveStraight(24_in);
      // Turn NNW
      if (autoSelector.alliance == Alliance::red) {
        chassis.turnAngle(-15_deg);
      } else {
        chassis.turnAngle(15_deg);
      }
      // Intake On
      // Drive to A1
      chassis.driveStraight(24_in);
      // Reverse to A2
      chassis.driveStraight(-24_in);
      // Turn N
      if (autoSelector.alliance == Alliance::red) {
        chassis.turnAngle(15_deg);
      } else {
        chassis.turnAngle(-15_deg);
      }
      // Reverse to A3
      chassis.driveStraight(-24_in);


      // Turn NE
      if (autoSelector.alliance == Alliance::red) {
        chassis.turnAngle(45_deg);
      } else {
        chassis.turnAngle(-45_deg);
      }
      // Drive to B2
      chassis.driveStraight(34_in);
      // Single Shot - 4 Position
      // Intake Reverse
      // Drive to C1
      chassis.driveStraight(34_in);

      // Reverse to between B2 and C1
      chassis.driveStraight(-17_in);
      // Turn to South
      if (autoSelector.alliance == Alliance::red) {
        chassis.turnAngle(135_deg);
      } else {
        chassis.turnAngle(-135_deg);
      }
      // Drive to betwewen B4 and C4
      chassis.driveStraight(60_in); // Might need to beef this up to skid fully up platform
    }


    // Red Alliance Side of Field
    // North
    // Ai B1 C1 Flags
    // A2 B2 C2 Flat Caps
    // A3 B3 C3 Tilted Caps
    // A4 B4 C4 Platforms
    // A5 B5 C5 Tilted Caps
    // A6 B6 C6 Expansion Zone, Flat Caps

    case (Tile::back): { // 2hf + 1lc + 1hc + plfm = 10pts
      // START: A5 Facing EAST
      // Intake On
      // Drive to C5
      chassis.driveStraight(48_in);
      // Intake Load Function


      // Turn North
      if (autoSelector.alliance == Alliance::red) {
        chassis.turnAngle(90_deg);
      } else {
        chassis.turnAngle(-90_deg);
      }
      // Reverse to between C5 and C6
      chassis.driveStraight(-12_in);
      // TwoBar to Mid
      // Reverse to C6
      chassis.driveStraight(-12_in);
      // DoubleShot - 3 Position


      // Drive ARC to between B6 and C6 - face SOUTH
      chassis.driveArc(12_in, 180_deg);
      // TwoBar to High
      // Reverse to between B6 and C5
      chassis.driveStraight(-12_in);
      // TwoBar to Low
      // Turn 360deg - face NORTH
      if (autoSelector.alliance == Alliance::red) {
        chassis.turnAngle(180_deg);
      } else {
        chassis.turnAngle(-180_deg);
      }
      // Drive to between B4 and C4
      chassis.driveStraight(24_in); // May need to beef this up to skid up platform
    }

    case (Tile::skills): { // 16pts?
      // START:A3 Facing EAST
      // Intake Forward
      // Drive forward to C3
      chassis.driveStraight(48_in);
      // Intake Load Function
      // Reverse to A3 (a bit east)
      chassis.driveStraight(-48_in);
      // Turn LEFT to face NORTH
      if (autoSelector.alliance == Alliance::red) {
        chassis.turnAngle(-90_deg);
      } else {
        chassis.turnAngle(90_deg);
      }
      // Double Shot - 1 Position


      // Drive to A2
      chassis.driveStraight(24_in);
      // Turn NNW
      if (autoSelector.alliance == Alliance::red) {
        chassis.turnAngle(-15_deg);
      } else {
        chassis.turnAngle(15_deg);
      }
      // Intake On
      // Drive to A1
      chassis.driveStraight(24_in);
      // Reverse to A2
      chassis.driveStraight(-24_in);
      // Turn N
      if (autoSelector.alliance == Alliance::red) {
        chassis.turnAngle(15_deg);
      } else {
        chassis.turnAngle(-15_deg);
      }
      // Reverse to A3
      chassis.driveStraight(-24_in);


      // Turn NE
      if (autoSelector.alliance == Alliance::red) {
        chassis.turnAngle(45_deg);
      } else {
        chassis.turnAngle(-45_deg);
      }
      // Drive to B2
      chassis.driveStraight(34_in);
      // Single Shot - 4 Position
      // Intake Reverse
      // Drive to C1
      chassis.driveStraight(34_in);

      // Reverse to A3
      chassis.driveStraight(-34_in);
      // Turn North
      if (autoSelector.alliance == Alliance::red) {
        chassis.turnAngle(-45_deg);
      } else {
        chassis.turnAngle(45_deg);
      }
      // Reverse to A4
      chassis.driveStraight(-24_in);
      // Turn East
      if (autoSelector.alliance == Alliance::red) {
        chassis.turnAngle(90_deg);
      } else {
        chassis.turnAngle(-90_deg);
      }
      // Climb Center Platform
      chassis.driveStraight(60_in); // Might need to beef this up to skid up platform

    }


    case (Tile::disabled): {
      // Intake On
      pros::Task::delay(500);
      // Intake Off

      // Uncomment for Domo-Auto
      //pros::Task::delay(12500);
      //chassis.drive.moveDistance(144_in);
    }
  }
}
