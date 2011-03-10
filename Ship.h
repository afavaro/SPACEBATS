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

struct Animation {
	aiQuaternion start;
	aiQuaternion end;
	GLfloat time, duration;
};

class Ship : public InputListener {
public:
	Ship(aiVector3D pos, aiMatrix3x3 basis, Camera* c);
	~Ship();
	
	void setProjectionAndView(float aspectRatio); 
	void handleEvent(sf::Event &event, const sf::Input &input);
	
	void roll(GLfloat rotation);

	void update(float tstep);

	Animation *anim;
	
	Model model;
	
	aiVector3D pos;
	aiQuaternion quat;
	
	Camera* cam;	

private:
	aiQuaternion neutral;
	aiQuaternion maxRollLeft;
	aiQuaternion maxRollRight;
	aiQuaternion maxPitchUp;
	aiQuaternion maxPitchDown;
};

#endif
