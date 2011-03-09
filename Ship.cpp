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
#define PITCH_ROTATION	-M_PI/15.0
#define ROLL_ROTATION	-M_PI/6.0

using namespace std;

Ship::Ship(aiVector3D pos, aiMatrix3x3 basis, Camera* c) {
	aiQuaternion adjust(aiVector3D(1,0,0), -M_PI / 2.0);
	neutral = aiQuaternion(aiVector3D(0, 1, 0), -M_PI / 2.0) * adjust;
	maxRollLeft = aiQuaternion(aiVector3D(0, 0, -1), -ROLL_ROTATION);
	maxRollRight = aiQuaternion(aiVector3D(0, 0, -1), ROLL_ROTATION);
	maxPitchUp = aiQuaternion(aiVector3D(1, 0, 0), PITCH_ROTATION);
	maxPitchDown = aiQuaternion(aiVector3D(1, 0, 0), -PITCH_ROTATION);

	this->pos = pos;
	this->cam = c;
	quat = neutral;
	anim = NULL;

	aiMatrix4x4 translation;
	aiMatrix4x4::Translation(pos, translation);
	aiMatrix4x4 transformation = translation * aiMatrix4x4(quat.GetMatrix());
	model.setTransformation(transformation);
}

Ship::~Ship() {}

void Ship::update(float tstep) {
	if (anim) {
		anim->time += tstep;
		if (anim->time > anim->duration) {
			quat = anim->end;
			delete anim; anim = NULL;
		}
		else aiQuaternion::Interpolate(quat, anim->start, anim->end, anim->time / anim->duration);

		aiMatrix4x4 translation;
		aiMatrix4x4::Translation(pos, translation);
		aiMatrix4x4 transformation = translation * aiMatrix4x4(quat.GetMatrix());
		model.setTransformation(transformation);
	}
}


void Ship::handleEvent(sf::Event &event, const sf::Input &input) {
	Animation *rot;
	switch (event.Type) {
		case sf::Event::KeyPressed: 
			switch(event.Key.Code){
				case sf::Key::A:
					if (anim == NULL && quat != maxRollLeft) {
						anim = new Animation();
						anim->start = quat;
						anim->end = maxRollLeft * neutral;
						anim->time = 0.0;
						anim->duration = DURATION;
					} else if(anim->end == neutral){
						rot = new Animation();
						rot->start = quat;
						rot->end = maxRollLeft * neutral;
						rot->time = 0.0;
						rot->duration = DURATION;
						delete anim;
						anim = rot;
					}
					break;
				case sf::Key::D:
					if (anim == NULL && quat != maxRollRight) {
						anim = new Animation();
						anim->start = quat;
						anim->end = maxRollRight * neutral;
						anim->time = 0.0;
						anim->duration = DURATION;
					}else if(anim->end == neutral){
						rot = new Animation();
						rot->start = quat;
						rot->end = maxRollRight * neutral;
						rot->time = 0.0;
						rot->duration = DURATION;
						delete anim;
						anim = rot;
					}
					break;
				case sf::Key::W:
					if (anim == NULL && quat != maxPitchUp) {
						anim = new Animation();
						anim->start = quat;
						anim->end = maxPitchUp * neutral;
						anim->time = 0.0;
						anim->duration = DURATION;
					} else if(anim->end == neutral){
						rot = new Animation();
						rot->start = quat;
						rot->end = maxPitchUp * neutral;
						rot->time = 0.0;
						rot->duration = DURATION;
						delete anim;
						anim = rot;
					}
					break;
				case sf::Key::S:
					if (anim == NULL && quat != maxPitchDown) {
						anim = new Animation();
						anim->start = quat;
						anim->end = maxPitchDown * neutral;
						anim->time = 0.0;
						anim->duration = DURATION;
					}else if(anim->end == neutral){
						rot = new Animation();
						rot->start = quat;
						rot->end = maxPitchDown * neutral;
						rot->time = 0.0;
						rot->duration = DURATION;
						delete anim;
						anim = rot;
					}
					break;
				default:
					break;
			}
			break;
		case sf::Event::KeyReleased: 
			Animation *back;
			switch(event.Key.Code){
				case sf::Key::A:
				case sf::Key::S:
				case sf::Key::W:
				case sf::Key::D:
					back = new Animation();
					back->start = quat;
					back->end = neutral;
					back->time = 0.0;
					back->duration = DURATION;
					delete anim;
					anim = back;
					break;
				default:
					break;
			}
		default:
			break;
	}
}
