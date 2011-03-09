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
using namespace std;

Ship::Ship(aiVector3D pos, aiMatrix3x3 basis, Camera* c) {
	neutral = aiQuaternion(aiVector3D(0, 1, 0), -M_PI / 2.0);
	maxRollLeft = aiQuaternion(aiVector3D(0, 0, -1), M_PI / 6.0);
	maxRollRight = aiQuaternion(aiVector3D(0, 0, -1), -M_PI / 6.0);

	this->pos = pos;
	this->cam = c;
	quat = neutral;
	anim = NULL;

	aiMatrix4x4 translation;
	aiMatrix4x4::Translation(pos, translation);
	aiMatrix4x4 rot;
	aiMatrix4x4::RotationX(-M_PI / 2.0, rot);
	aiMatrix4x4 transformation = translation * aiMatrix4x4(quat.GetMatrix()) * rot;
	model.setTransformation(transformation);

}

Ship::~Ship() {}

/*
GLfloat Ship::quadEaseIn(GLfloat timePassed, GLfloat power){
	return pow(timePassed, power);
}

GLfloat Ship::quadEaseOut(GLfloat timePassed, GLfloat power){
	return 1- quadEaseIn(1.0 - timePassed, power);
}

GLfloat Ship::quadEaseInOut(GLfloat timePassed, GLfloat power){
	if(timePassed < 0.5) return quadEaseIn(2.0*timePassed, power)/2.0;
	return quadEaseOut(2.0*(timePassed-0.5),power)/2.0 + 0.5;
}

void Ship::easeToNewPosition(){
	if(time == duration) return;
	rot = change*quadEaseInOut(time/duration, 2.0) + startVal;
	//rot = quadEaseInOut(time/duration);
	time++;
}

void Ship::roll(GLfloat rotation){
	time = 0;
	startVal = rot;
	change = rotation;
	rotationDebt += rotation;
	easeToNewPosition();
}
*/

void Ship::update(float tstep) {
	if (anim) {
		anim->time += tstep;
		if (anim->time > anim->duration) {
			quat = anim->end;
			delete anim; anim = NULL;
			return;
		}

		aiQuaternion::Interpolate(quat, anim->start, anim->end, anim->time / anim->duration);

		aiMatrix4x4 translation;
		aiMatrix4x4::Translation(pos, translation);
		aiMatrix4x4 rot;
		aiMatrix4x4::RotationX(-M_PI / 2.0, rot);
		aiMatrix4x4 transformation = translation * aiMatrix4x4(quat.GetMatrix()) * rot;
		model.setTransformation(transformation);
	}
}


void Ship::handleEvent(sf::Event &event, const sf::Input &input) {
	switch (event.Type) {
		case sf::Event::KeyPressed: 
			switch(event.Key.Code){
				case sf::Key::A:
					if (anim == NULL && quat != maxRollLeft) {
						anim = new Animation();
						anim->start = quat;
						anim->end = maxRollLeft;
						anim->time = 0.0;
						anim->duration = 0.2;
					}
					break;
				case sf::Key::D:
					if (anim == NULL && quat != maxRollRight) {
						anim = new Animation();
						anim->start = quat;
						anim->end = maxRollRight;
						anim->time = 0.0;
						anim->duration = 0.2;
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
				case sf::Key::D:
					if (anim == NULL) break;
					back = new Animation();
					back->start = quat;
					back->end = anim->start;
					back->time = anim->duration - anim->time;
					back->duration = anim->duration;
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
