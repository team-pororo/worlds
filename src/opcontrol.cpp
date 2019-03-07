#include "main.h"
#include "chassis.h"
#include "intake.h"
#include "puncher.h"
#include "autohandler.h"

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
extern Controller controller;

extern AutoHandler autohandler;


void opcontrol() {
	for (int i = 0;; ++i) {
		chassis.teleop(controller);
		puncher.teleop(intake);
		intake.teleop();

		if (i % 10 == 0) {
			autohandler.interface(GameState::Teleop);
		}

		pros::Task::delay(10);
	}
}
