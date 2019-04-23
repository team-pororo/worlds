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

void autonomous() {
  switch (autoSelector.tile) {
    case (Tile::front): {
      // Retrieve ball from tilted cap
      intake.moveSpeed(-200);
      chassis.drive.moveDistance(36_in);
      chassis.drive.moveDistance(-36_in);

      if (autoSelector.alliance == Alliance::red) {
        chassis.drive.turnAngle(-90_deg);
      } else {
        chassis.drive.turnAngle(90_deg);
      }

      shotHandler.doubleShoot(1);
      shotHandler.waitUntilSettled();

      // Intake balls, get low flag
      intake.moveSpeed(-200);

      chassis.drive.moveDistance(36_in);

      if (autoSelector.alliance == Alliance::red) {
        chassis.drive.turnAngle(-20_deg);
      } else {
        chassis.drive.turnAngle(20_deg);
      }

      chassis.drive.moveDistance(12_in);

      // Back up to starting tile
      chassis.drive.moveDistance(-12_in);

      if (autoSelector.alliance == Alliance::red) {
        chassis.drive.turnAngle(20_deg);
      } else {
        chassis.drive.turnAngle(-20_deg);
      }

      chassis.drive.moveDistance(-26_in);

      // Go to flat cap
      if (autoSelector.alliance == Alliance::red) {
        chassis.drive.turnAngle(-45_deg);
      } else {
        chassis.drive.turnAngle(45_deg);
      }

      intake.moveSpeed(200);

      chassis.drive.moveDistance(18_in);
      shotHandler.shoot();

    }

    case (Tile::back): {
      // Retrieve ball from tilted cap
      intake.moveSpeed(-200);
      chassis.drive.moveDistance(36_in);

      // Pick up flat cap in two-bar

      if (autoSelector.alliance == Alliance::red) {
        chassis.drive.turnAngle(-90_deg);
      } else {
        chassis.drive.turnAngle(90_deg);
      }

      chassis.drive.moveDistance(-20_in);
      twobar.moveTo(1);

      // Drive to post

      chassis.drive.moveDistance(8_in);

      if (autoSelector.alliance == Alliance::red) {
        chassis.drive.turnAngle(-90_deg);
      } else {
        chassis.drive.turnAngle(90_deg);
      }

      // Score cap

      chassis.drive.moveDistance(48_in);
      twobar.moveTo(2);
      twobar.waitUntilSettled();
      chassis.drive.moveDistance(-12_in);
      twobar.moveTo(0);

      // Turn to shoot
      if (autoSelector.alliance == Alliance::red) {
        chassis.drive.turnAngle(-23_deg);
      } else {
        chassis.drive.turnAngle(23_deg);
      }

      // Shoot
      shotHandler.doubleShoot(3);
      shotHandler.waitUntilSettled();

      // Line up for platform climb
      if (autoSelector.alliance == Alliance::red) {
        chassis.drive.turnAngle(23_deg);
      } else {
        chassis.drive.turnAngle(-23_deg);
      }

      chassis.drive.moveDistance(-12_in);
      chassis.drive.moveDistance(43_in);


      if (autoSelector.alliance == Alliance::red) {
        chassis.drive.turnAngle(90_deg);
      } else {
        chassis.drive.turnAngle(-90_deg);
      }

      chassis.drive.moveDistance(-10_in);

      // Climb platform
      chassis.drive.moveDistance(42_in);
    }

    case (Tile::skills): {
      // Retrieve ball from tilted cap
      intake.moveSpeed(-200);
      chassis.drive.moveDistance(36_in);

      // Pick up flat cap in two-bar

      if (autoSelector.alliance == Alliance::red) {
        chassis.drive.turnAngle(-90_deg);
      } else {
        chassis.drive.turnAngle(90_deg);
      }

      chassis.drive.moveDistance(-20_in);
      twobar.moveTo(1);

      // Drive to post

      chassis.drive.moveDistance(8_in);

      if (autoSelector.alliance == Alliance::red) {
        chassis.drive.turnAngle(-90_deg);
      } else {
        chassis.drive.turnAngle(90_deg);
      }

      // Score cap

      chassis.drive.moveDistance(48_in);
      twobar.moveTo(2);
      twobar.waitUntilSettled();
      chassis.drive.moveDistance(-12_in);
      twobar.moveTo(0);

      // Turn to shoot
      if (autoSelector.alliance == Alliance::red) {
        chassis.drive.turnAngle(-23_deg);
      } else {
        chassis.drive.turnAngle(23_deg);
      }

      // Shoot
      shotHandler.doubleShoot(3);
      shotHandler.waitUntilSettled();

      // Line up for platform climb
      if (autoSelector.alliance == Alliance::red) {
        chassis.drive.turnAngle(23_deg);
      } else {
        chassis.drive.turnAngle(-23_deg);
      }

      chassis.drive.moveDistance(-12_in);
      chassis.drive.moveDistance(43_in);


      if (autoSelector.alliance == Alliance::red) {
        chassis.drive.turnAngle(90_deg);
      } else {
        chassis.drive.turnAngle(-90_deg);
      }

      chassis.drive.moveDistance(-10_in);

      // Climb platform
      chassis.drive.moveDistance(60_in);

    }

    case (Tile::disabled): {
      pros::Task::delay(13000);
      chassis.drive.moveDistance(144_in);
    }
  }
}
