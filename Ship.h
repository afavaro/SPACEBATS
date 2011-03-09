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

class Ship : public InputListener {
public:
	Ship(aiVector3D pos, aiMatrix3x3 basis, Camera* c);
	~Ship();
	
	void setProjectionAndView(float aspectRatio); 
	void handleEvent(sf::Event &event, const sf::Input &input);
	
	GLfloat quadEaseIn(GLfloat timePassed, GLfloat power);
	
	GLfloat quadEaseOut(GLfloat timePassed, GLfloat power);
	
	GLfloat quadEaseInOut(GLfloat timePassed, GLfloat power);
	
	void easeToNewPosition();
	void roll(GLfloat rotation);
	
	Model model;
	
	GLfloat rot;
	aiVector3D pos;
	aiMatrix3x3 basis;
	aiMatrix4x4 transformation;
	aiQuaternion shipQuat;
	
	GLfloat time, duration, change, startVal;
	GLfloat rotationDebt;
	Camera* cam;
	
};

#endif