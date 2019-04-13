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
      // Move the two-bar out of the way, aim, move balls out of the trajectory, pre-arm puncher
      self->twobar.avoidPuncherPath();
      self->angler.moveToAngle(self->targetAngle0);
      self->puncher.ready();
      self->intake.clear();

      self->twobar.waitUntilSettled();
      self->angler.waitUntilSettled();
      self->intake.waitUntilSettled();
      self->puncher.waitUntilSettled();

      // Punch!
      self->puncher.punch();
      self->puncher.waitUntilSettled();


      // Move two-bar back down
      self->twobar.returnToInitial();

    } else if (self->mode == ShotHandler::Mode::load) {

      self->intake.ready();

    } else if (self->mode == ShotHandler::Mode::doubleShoot) {
      // Move the two-bar out of the way, aim, move balls out of the trajectory, pre-arm puncher
      self->twobar.avoidPuncherPath();
      self->angler.moveToAngle(self->targetAngle0);
      self->puncher.ready();
      self->intake.clear();

      self->twobar.waitUntilSettled();
      self->angler.waitUntilSettled();
      self->intake.waitUntilSettled();
      self->puncher.waitUntilSettled();

      // Punch!
      self->puncher.punch();
      self->puncher.waitUntilSettled();

      // Aim for second target, load second ball, pre-arm puncher
      self->angler.moveToAngle(self->targetAngle1);
      self->intake.load();
      self->puncher.ready();

      self->twobar.waitUntilSettled();
      self->angler.waitUntilSettled();
      self->puncher.waitUntilSettled();

      // Punch!
      self->puncher.punch();
      self->puncher.waitUntilSettled();

      // Move two-bar back down
      self->twobar.returnToInitial();
    }

    self->settled = true;
    //pros::c::delay(10);

  }
}

void ShotHandler::runPuncherReady(void *self_p) {
  ShotHandler* self = (ShotHandler*)self_p;
  while (true) {
    if (self->intake.ballPresent(BallPosition::puncher)) {
      if (self->puncher.state == PuncherState::idle) {
        self->puncher.ready();
      }
    }
    pros::Task::delay(20);
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
  if (zone0.changedToPressed()) { // Zone X, Very Front
    doubleShoot(55, 40);
  }
  if (zone1.changedToPressed()) { // Zone Y, Front Tile
    doubleShoot(40, 28);
  }
  if (zone2.changedToPressed()) { // Zone A, Center/Back Tile
    doubleShoot(40, 28);
  }
  if (zone3.changedToPressed()) { // Zone B, Very Back
    doubleShoot(45, 30);
  }
  if (legacy.changedToPressed()) {
    shoot(45);
  }
  if (otmr.changedToPressed()) {
    load();
  }
}
