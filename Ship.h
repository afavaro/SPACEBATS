/*
 *  Ship.h
 *  SPACEBATS
 *
 *  Created by D! on 3/8/11.
 *  Copyright 2011 Stanford University. All rights reserved.
 */

#ifndef SHIP_H
#define SHIP_H

#include "Framework.h"
#include "InputListener.h"
#include "Model.h"
#include "Camera.h"

#include <cmath>

struct Rotation {
	aiQuaternion start;
	aiQuaternion end;
	GLfloat time, duration;
};

class Ship : public InputListener {
public:
	Ship(aiVector3D pos, aiMatrix3x3 basis, Camera* c);
	~Ship();
	
	void handleEvent(sf::Event &event, const sf::Input &input);
	void update(float tstep);
	
	Rotation *curRot;
	Model model;
	aiVector3D pos;
	aiVector3D velocity;
	aiVector3D acceleration;
	aiQuaternion quat;
	
	Camera* cam;	

private:
	void updatePosition(float tstep);
	void updateRotation(float tstep);

	void setRotation(aiQuaternion end);

	aiQuaternion neutral;
	aiQuaternion maxRollLeft;
	aiQuaternion maxRollRight;
	aiQuaternion maxPitchUp;
	aiQuaternion maxPitchDown;
};

#endif
