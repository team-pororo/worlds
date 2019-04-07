#include "main.h"
#include "chassis.h"
#include "intake.h"
#include "puncher.h"
#include "twobar.h"
#include "shothandler.h"
#include "autoselect.h"
#include "watchdog.h"
#include "controllerLCD.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

Controller controller = Controller(ControllerId::master);

Chassis chassis = Chassis(controller);
Intake intake;
Puncher puncher;
Angler angler;
TwoBar twobar;
ShotHandler shotHandler = ShotHandler(puncher, angler, intake, twobar);
AutoSelector autoSelector;
Watchdog watchdog = Watchdog(controller);
ControllerLCD controllerLCD = ControllerLCD(controller, autoSelector, watchdog);

void btn0() {
	autoSelector.btn0();
}
void btn1() {
	autoSelector.btn1();
}
void btn2() {
	autoSelector.btn2();
}

void initialize() {
	pros::Task PIDtask(Angler::runPID, &angler);
	pros::Task VisTask(Intake::runVision, &intake);
	//pros::Task TwoBarTask(TwoBar::runPID, &twobar);
	twobar.drop();

	pros::task_t shootTask = pros::c::task_create(ShotHandler::runShoot, &shotHandler, TASK_PRIORITY_DEFAULT,
                              TASK_STACK_DEPTH_DEFAULT, "ShotHandler");
	//pros::Task shootTask(ShotHandler::runShoot, &shotHandler);

	shotHandler.shootTask = shootTask;

	pros::task_t punchTask = pros::c::task_create(Puncher::runPID, &puncher, TASK_PRIORITY_DEFAULT,
                              TASK_STACK_DEPTH_DEFAULT, "Puncher");

	puncher.punchTask = punchTask;

	pros::lcd::initialize();
	if (!pros::competition::is_connected()) {
		puncher.motor.moveAbsolute(10, 100); // make sure the gears are meshing
		controllerLCD.init();
	}
	pros::lcd::register_btn0_cb(btn0);
	pros::lcd::register_btn1_cb(btn1);
	pros::lcd::register_btn2_cb(btn2);

	twobar.motor.moveAbsolute(-15, 100);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	puncher.motor.moveAbsolute(10, 100); // make sure the gears are meshing
	controllerLCD.init();
	autoSelector.runSelector();
}
