#include "main.h"
#include "chassis.h"
#include "intake.h"
#include "puncher.h"
#include "angler.h"
#include "twobar.h"
#include "shothandler.h"
#include "autoselect.h"
#include "watchdog.h"
#include "controllerLCD.h"

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

extern Chassis chassis;
extern Intake intake;
extern Puncher puncher;
extern Angler angler;
extern TwoBar twobar;
extern ShotHandler shotHandler;
extern Controller controller;
extern AutoSelector autoSelector;
extern Watchdog watchdog;
extern ControllerLCD controllerLCD;

void opcontrol() {
	watchdog.start();
	//angler.moveToAngle(55); // Minimum angle for working puncher retraction
	//angler.waitUntilSettled();
	for (int i = 0;; ++i) {
		chassis.teleop();
		shotHandler.teleop();
		intake.teleop();
		twobar.teleop();
		watchdog.teleop();
		autoSelector.teleop();
		controllerLCD.teleop();

		/*if (controller.getDigital(ControllerDigital::B)) {
			angler.moveToAngle(55);
		} else if (controller.getDigital(ControllerDigital::A)) {
			angler.moveToAngle(45);
		} else if (controller.getDigital(ControllerDigital::Y)) {
			angler.moveToAngle(35);
		} else if (controller.getDigital(ControllerDigital::X)) {
			angler.moveToAngle(30);
		}*/

		if (controller.getDigital(ControllerDigital::R2)) {
			printf("opcontrol() shootTask = %10d\n", shotHandler.shootTask);

		}

		pros::Task::delay(20);
	}
}
