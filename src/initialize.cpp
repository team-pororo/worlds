#include "main.h"
#include "chassis.h"
#include "intake.h"
#include "puncher.h"
#include "doubleshots.h"
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
DoubleShotHandler doubleShotHandler = DoubleShotHandler(puncher, angler, intake);
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
	pros::lcd::initialize();
	if (!pros::competition::is_connected()) {
		controllerLCD.init();
	}
	pros::lcd::register_btn0_cb(btn0);
	pros::lcd::register_btn1_cb(btn1);
	pros::lcd::register_btn2_cb(btn2);
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
	controllerLCD.init();
	autoSelector.runSelector();
}
