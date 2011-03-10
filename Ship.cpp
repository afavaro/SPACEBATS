/*
 *  Ship.cpp
 *  CS248-Assn4
 *
 *  Created by D! on 3/8/11.
 *  Copyright 2011 Stanford University. All rights reserved.
 *
 */
//assrow-> glcolv
#include "Ship.h"
#include <iostream>
#include <math.h>

#define DURATION 0.2
#define THRUST 100.0

#define EASE(t) ((t) * (t) * (3.0 - 2.0 * (t)))

#define PITCH_ROTATION	M_PI/15.0
#define ROLL_ROTATION	M_PI/6.0

using namespace std;

Ship::Ship(aiVector3D pos, aiMatrix3x3 basis, Camera* c) {
	aiQuaternion adjust(aiVector3D(1, 0, 0), -M_PI / 2.0);
	neutral = aiQuaternion(aiVector3D(0, 1, 0), -M_PI / 2.0) * adjust;
	maxRollLeft = aiQuaternion(aiVector3D(0, 0, -1), -ROLL_ROTATION);
	maxRollRight = aiQuaternion(aiVector3D(0, 0, -1), ROLL_ROTATION);
	maxPitchUp = aiQuaternion(aiVector3D(1, 0, 0), PITCH_ROTATION);
	maxPitchDown = aiQuaternion(aiVector3D(1, 0, 0), -PITCH_ROTATION);

	this->pos = pos;
	acceleration = velocity = aiVector3D(0, 0, 0);
	this->cam = c;
	quat = neutral;
	curRot = NULL;

	aiMatrix4x4 translation;
	aiMatrix4x4::Translation(pos, translation);
	aiMatrix4x4 transformation = translation * aiMatrix4x4(quat.GetMatrix());
	model.setTransformation(transformation);
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
			aiQuaternion::Interpolate(quat,
					curRot->start, curRot->end, EASE(t));
		}
	}
}

void Ship::updatePosition(float tstep) {
	pos += velocity * tstep;
	velocity += acceleration * tstep;
}

void Ship::update(float tstep) {
	updateRotation(tstep);
	updatePosition(tstep);

	aiMatrix4x4 translation;
	aiMatrix4x4::Translation(pos, translation);
	aiMatrix4x4 transformation = translation * aiMatrix4x4(quat.GetMatrix());
	model.setTransformation(transformation);
}

void Ship::setRotation(aiQuaternion rot) {
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
					acceleration = aiVector3D(-THRUST, 0, 0);
					if ((curRot == NULL && quat != maxRollLeft) || curRot->end == neutral)
						setRotation(maxRollLeft);
					break;
				case sf::Key::D:
					acceleration = aiVector3D(THRUST, 0, 0);
					if ((curRot == NULL && quat != maxRollRight) || curRot->end == neutral)
						setRotation(maxRollRight);
					break;
				case sf::Key::W:
					acceleration = aiVector3D(0, THRUST, 0);
					if ((curRot == NULL && quat != maxPitchUp) || curRot->end == neutral)
						setRotation(maxPitchUp);
					break;
				case sf::Key::S:
					acceleration = aiVector3D(0, -THRUST, 0);
					if ((curRot == NULL && quat != maxPitchDown) || curRot->end == neutral)
						setRotation(maxPitchDown);
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
					acceleration = velocity = aiVector3D(0, 0, 0);
					setRotation(aiQuaternion(1, 0, 0, 0));
					break;
				default:
					break;
			}
		default:
			break;
	}
}
