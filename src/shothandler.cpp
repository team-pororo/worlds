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
  //printf("ShotHandler::runShoot() shootTask = %10d\n", self->shootTask);

  while (pros::c::task_notify_take(true, TIMEOUT_MAX)) {
  //while (true) {
    self->settled = false;



    //printf("ShotHandler::runShoot() still alive\n");



    if (self->mode == ShotHandler::Mode::shoot) {
      // Move the two-bar out of the way, aim, move balls out of the trajectory, pre-arm puncher
      self->twobar.avoidPuncherPath();
      self->angler.moveToAngle(false, self->targetPos);
      self->puncher.ready();
      self->intake.clear();

      self->twobar.waitUntilSafeToShoot();
      self->angler.waitUntilSettled();
      self->intake.waitUntilSettled();
      self->puncher.waitUntilSettled();

      // Punch!
      self->puncher.punch();
      self->puncher.waitUntilSettled();


      // Move two-bar back down
      self->twobar.returnToInitial();
      self->angler.moveToAngle(false, 4); // return to resting angle

    } else if (self->mode == ShotHandler::Mode::load) {

      self->intake.ready();

    } else if (self->mode == ShotHandler::Mode::doubleShoot) {
      // Move the two-bar out of the way, aim, move balls out of the trajectory, pre-arm puncher
      self->twobar.avoidPuncherPath();
      self->angler.moveToAngle(true, self->targetPos);
      self->puncher.ready();
      self->intake.clear();

      self->twobar.waitUntilSafeToShoot();
      self->angler.waitUntilSettled();
      self->intake.waitUntilSettled();
      self->puncher.waitUntilSettled();

      // Punch!
      self->puncher.punch();
      self->puncher.waitUntilSettled();

      // Aim for second target, load second ball, pre-arm puncher
      self->angler.moveToAngle(false, self->targetPos);
      self->puncher.ready();

      pros::Task::delay(100);
      self->intake.load();

      self->angler.waitUntilSettled();
      self->puncher.waitUntilSettled();

      // Punch!
      self->puncher.punch();
      self->puncher.waitUntilSettled();

      // Move two-bar back down
      self->twobar.returnToInitial();
      self->angler.moveToAngle(false, 4); // return to resting angle
    }

    self->settled = true;
    //pros::c::delay(10);

  }
}

void ShotHandler::runPuncherReady(void *self_p) {
  //pros::Task::delay(1000);
  ShotHandler* self = (ShotHandler*)self_p;
  while (true) {
    if (self->intake.ballPresent(BallPosition::puncher)) {
      if (self->puncher.state == PuncherState::idle) {
        self->puncher.ready();
      }
    }
    if (self->puncher.state == PuncherState::punch || self->puncher.state == PuncherState::pullbackAndPunch) {
      pros::Task::delay(100); // wait for ball to leave puncher after fire
    }
    pros::Task::delay(20);

    if (self->puncher.state == PuncherState::idle) {
      pros::lcd::print(4, "Puncher: Idle");
    } else if (self->puncher.state == PuncherState::ready) {
      pros::lcd::print(4, "Puncher: Ready");
    } else if (self->puncher.state == PuncherState::punch) {
      pros::lcd::print(4, "Puncher: Punching");
    } else if (self->puncher.state == PuncherState::pullback) {
      pros::lcd::print(4, "Puncher: Pulling back");
    } else if (self->puncher.state == PuncherState::pullbackAndPunch) {
      pros::lcd::print(4, "Puncher: Pulling back and punching");
    }
  }
}

void ShotHandler::shoot() {
  targetPos = 4; // LEGACY position
  mode = Mode::shoot;

  if (shootTask) {
    pros::c::task_notify(shootTask);
    //shootTask->notify_clear();
  }
}

void ShotHandler::doubleShoot(int pos) {
  targetPos = pos;
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
    doubleShoot(0);
  }
  if (zone1.changedToPressed()) { // Zone Y, Front Tile
    doubleShoot(1);
  }
  if (zone2.changedToPressed()) { // Zone A, Center/Back Tile
    doubleShoot(2);
  }
  if (zone3.changedToPressed()) { // Zone B, Very Back
    doubleShoot(3);
  }
  if (legacy.changedToPressed()) {
    shoot();
  }
  if (otmr.changedToPressed()) {
    load();
  }
}
