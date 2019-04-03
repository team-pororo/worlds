#include "shothandler.h"
#include "main.h"
#include "intake.h"
#include "puncher.h"
#include "angler.h"
#include "twobar.h"

bool ShotHandler::isSettled() {
  return settled;
}

void ShotHandler::waitUntilSettled() {
  while (!isSettled()) {
    pros::Task::delay(20);
  }
}

void ShotHandler::runShoot(void* self_p) {
  ShotHandler* self = (ShotHandler*)self_p;
  printf("ShotHandler::runShoot() shootTask = %10d\n", self->shootTask);

  while (pros::c::task_notify_take(true, TIMEOUT_MAX)) {
  //while (true) {
    self->settled = false;



    printf("ShotHandler::runShoot() still alive\n");



    if (self->mode == ShotHandler::Mode::shoot) {
      // Move the two-bar out of the way, aim, move balls out of the trajectory
      self->twobar.moveTo(1);
      self->angler.moveToAngle(self->targetAngle0);

      //self->intake.moveSpeed(200);
      //while (self->intake.ballPresent(BallPosition::trajectory)) {
      //  pros::Task::delay(20);
      //}
      //self->intake.moveSpeed(0);

      self->twobar.waitUntilSettled();
      self->angler.waitUntilSettled();

      self->puncher.punch();
      self->puncher.waitUntilSettled();

    } else if (self->mode == ShotHandler::Mode::load) {

      /*self->intake.moveSpeed(-200);
      while (!self->intake.ballPresent(BallPosition::puncher)) {
        pros::Task::delay(20);
      }
      self->intake.moveSpeed(200);
      while (self->intake.ballPresent(BallPosition::trajectory)) {
        pros::Task::delay(20);
      }
      self->intake.moveSpeed(0);*/

    } else if (self->mode == ShotHandler::Mode::doubleShoot) {
      // Move the two-bar out of the way, aim, move balls out of the trajectory
      self->twobar.moveTo(1);
      self->angler.moveToAngle(self->targetAngle0);

      /*self->intake.moveSpeed(200);
      while (self->intake.ballPresent(BallPosition::trajectory)) {
        pros::Task::delay(20);
      }
      self->intake.moveSpeed(0);*/

      self->twobar.waitUntilSettled();
      self->angler.waitUntilSettled();

      self->puncher.punch();
      self->puncher.waitUntilSettled();

      self->twobar.moveTo(1);
      self->angler.moveToAngle(self->targetAngle1);

      self->intake.moveSpeed(-200);
      /*while (self->intake.ballPresent(BallPosition::puncher)) {
        pros::Task::delay(20);
      }*/
      pros::Task::delay(1000);
      self->intake.moveSpeed(0);



      /*self->intake.moveSpeed(200);
      while (self->intake.ballPresent(BallPosition::trajectory)) {
        pros::Task::delay(20);
      }
      self->intake.moveSpeed(0);*/

      self->twobar.waitUntilSettled();
      self->angler.waitUntilSettled();

      self->puncher.punch();
      self->puncher.waitUntilSettled();
    }

    self->settled = true;
    //pros::c::delay(10);

  }
}

void ShotHandler::shoot(double angle) {
  targetAngle0 = angle;
  mode = Mode::shoot;

  if (shootTask) {
    pros::c::task_notify(shootTask);
    //shootTask->notify_clear();
  }
}

void ShotHandler::doubleShoot(double angle0, double angle1) {
  targetAngle0 = angle0;
  targetAngle1 = angle1;
  mode = Mode::doubleShoot;

  if (shootTask) {
    pros::c::task_notify(shootTask);
  }
}

void ShotHandler::load() {
  mode = Mode::load;

  if (shootTask) {
    pros::c::task_notify(shootTask);
  }
}

void ShotHandler::teleop() {
  if (zone0.changedToPressed()) {
    doubleShoot(45, 30);
  }
  if (zone1.changedToPressed()) {
    doubleShoot(45, 30);
  }
  if (zone2.changedToPressed()) {
    doubleShoot(45, 30);
  }
  if (zone3.changedToPressed()) {
    doubleShoot(45, 30);
  }
  if (legacy.changedToPressed()) {
    shoot(45);
  }
  if (otmr.changedToPressed()) {
    load();
  }
}
