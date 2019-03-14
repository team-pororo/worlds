#include "main.h"
#include "chassis.h"
#include "intake.h"
#include "puncher.h"
#include "angler.h"
#include "doubleshots.h"
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
extern Angler angler;
extern DoubleShotHandler doubleShotHandler;
extern Controller controller;

extern AutoHandler autohandler;


void opcontrol() {
	angler.moveToAngle(55); // Minimum angle for working puncher retraction
	angler.waitUntilSettled();
	for (int i = 0;; ++i) {
		chassis.teleop(controller);
		doubleShotHandler.teleop();
		intake.teleop();

		if (i % 10 == 0) {
			autohandler.interface(GameState::Teleop);
			pros::lcd::print(5, "Intake Ball: %04d", intake.ballPresent(BallPosition::intake));
			pros::lcd::print(6, "Trajectory Ball: %04d", intake.ballPresent(BallPosition::trajectory));
			pros::lcd::print(7, "Puncher Ball: %04d", intake.ballPresent(BallPosition::puncher));
		}

		pros::Task::delay(10);
	}
}
