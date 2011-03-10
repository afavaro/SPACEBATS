
#include "Ship.h"
#include <iostream>
#include <math.h>

#define DURATION 0.2
#define THRUST 150.0
#define DRAG 200.0

#define EASE(t) ((t) * (t) * (3.0 - 2.0 * (t)))

#define PITCH_ROTATION	M_PI/15.0
#define ROLL_ROTATION	M_PI/6.0

#define BOUNDARY_X 25.0
#define BOUNDARY_Y 22.0
#define DELTA 0.01

using namespace std;

Ship::Ship(btVector3 pos, Camera* c) {
	btQuaternion adjust(btVector3(1, 0, 0), -M_PI / 2.0);
	neutral = btQuaternion(btVector3(0, 1, 0), -M_PI / 2.0) * adjust;
	maxRollLeft = btQuaternion(btVector3(0, 0, -1), -ROLL_ROTATION);
	maxRollRight = btQuaternion(btVector3(0, 0, -1), ROLL_ROTATION);
	maxPitchUp = btQuaternion(btVector3(1, 0, 0), PITCH_ROTATION);
	maxPitchDown = btQuaternion(btVector3(1, 0, 0), -PITCH_ROTATION);

	this->pos = pos;
	acceleration = velocity = btVector3(0, 0, 0);
	this->cam = c;
	quat = neutral;
	curRot = NULL;
	isStopping = false;
}

Ship::~Ship() {}

void Ship::updateRotation(float tstep) {
	if (curRot) {
		curRot->time += tstep;
		if (curRot->time > curRot->duration) {
			quat = curRot->end;
			delete curRot; curRot = NULL;
		}
		else {
			float t = curRot->time / curRot->duration;
			quat = curRot->start.slerp(curRot->end, EASE(t));
		}
	}
}


void Ship::updatePosition(float tstep) {
	pos += velocity * tstep;
	velocity += acceleration * tstep;

	if (pos.x() > BOUNDARY_X) {
		pos.setX(BOUNDARY_X - DELTA);
		if (velocity.x() > 0) velocity.setX(0);
		if (acceleration.x() > 0) acceleration.setX(0);
	}
	if (pos.x() < -BOUNDARY_X) {
		pos.setX(-BOUNDARY_X + DELTA);
		if (velocity.x() < 0) velocity.setX(0);
		if (acceleration.x() < 0) acceleration.setX(0);
	}
	if (pos.y() > BOUNDARY_Y) {
		pos.setY(BOUNDARY_Y - DELTA);
		if (velocity.y() > 0) velocity.setY(0);
		if (acceleration.y() > 0) acceleration.setY(0);
	}
	if (pos.y() < -BOUNDARY_Y) {
		pos.setY(-BOUNDARY_Y + DELTA);
		if (velocity.y() < 0) velocity.setY(0);
		if (acceleration.y() < 0) acceleration.setY(0);
	}

	if (isStopping && velocity.length() < 1) {
		velocity = acceleration = btVector3(0, 0, 0);
		isStopping = false;
	}
}

void Ship::update(float tstep) {
	updateRotation(tstep);
	updatePosition(tstep);

	btTransform transform(quat, pos);
	model.setTransformation(transform);
}

void Ship::setRotation(btQuaternion rot) {
	delete curRot;
	curRot = new Rotation();
	curRot->start = quat;
	curRot->end = rot * neutral;
	curRot->time = 0.0;
	curRot->duration = DURATION;
}



void Ship::handleEvent(sf::Event &event, const sf::Input &input) {
	switch (event.Type) {
		case sf::Event::KeyPressed: 
			switch(event.Key.Code){
				case sf::Key::A:
					if ((curRot == NULL && quat != maxRollLeft) || curRot->end == neutral)
						setRotation(maxRollLeft);
					isStopping = false;
					acceleration = btVector3(-THRUST, 0, 0);
					break;
				case sf::Key::D:
					if ((curRot == NULL && quat != maxRollRight) || curRot->end == neutral)
						setRotation(maxRollRight);
					isStopping = false;
					acceleration = btVector3(THRUST, 0, 0);
					break;
				case sf::Key::W:
					if ((curRot == NULL && quat != maxPitchUp) || curRot->end == neutral)
						setRotation(maxPitchUp);
					isStopping = false;
					acceleration = btVector3(0, THRUST, 0);
					break;
				case sf::Key::S:
					if ((curRot == NULL && quat != maxPitchDown) || curRot->end == neutral)
						setRotation(maxPitchDown);
					isStopping = false;
					acceleration = btVector3(0, -THRUST, 0);
					break;
				default:
					break;
			}
			break;
		case sf::Event::KeyReleased: 
			switch(event.Key.Code){
				case sf::Key::A:
				case sf::Key::S:
				case sf::Key::W:
				case sf::Key::D:
					if (velocity.length() > 0) {
						isStopping = true;
						acceleration = -DRAG * velocity.normalized();
					}
					setRotation(btQuaternion(0, 0, 0, 1));
					break;
				default:
					break;
			}
		default:
			break;
	}
}
