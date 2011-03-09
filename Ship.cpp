/*
 *  Ship.cpp
 *  CS248-Assn4
 *
 *  Created by D! on 3/8/11.
 *  Copyright 2011 Stanford University. All rights reserved.
 *
 */

#include "Ship.h"
#include <iostream>
using namespace std;

Ship::Ship(aiVector3D pos, aiMatrix3x3 basis, Camera* c) {
	this->pos = pos;
	this->basis = basis;
	this->cam = c;
	this->rot = 0.0;
	shipQuat = aiQuaternion(aiVector3D(1,0,0), 0);
	duration = 30;
	rotationDebt = 0;
	time = duration;
}

Ship::~Ship() {}

void Ship::setProjectionAndView(float aspectRatio) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspectRatio, 0.1, 500.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(pos.x, pos.y, pos.z,
			  pos.x + basis.a3, pos.y + basis.b3, pos.z + basis.c3,
			  basis.a2, basis.b2, basis.c2);
}	

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

void Ship::handleEvent(sf::Event &event, const sf::Input &input) {
	aiVector3D thrust(3.0*basis.a3,3.0*basis.b3,3.0*basis.c3);
	aiMatrix3x3 posMatX, basisMatX;	
	switch (event.Type) {
		case sf::Event::KeyPressed: 
			switch(event.Key.Code){
				case sf::Key::Up: 
					pos += thrust;
					this->cam->pull(pos, thrust);
					break;
				case sf::Key::Down: 
					pos -= thrust;
					this->cam->pull(pos, -thrust);
					break;
				case sf::Key::Left: 
					pos.x -= 1.0;
					roll(30.0);
					this->cam->pull(pos, aiVector3D(-1.0,0,0));
					break;
				case sf::Key::Right: 
					//while(input.IsKeyDown(sf::Key::Right)){
					pos.x += 1.0;
					roll(-30.0);
					this->cam->pull(pos, aiVector3D(1.0,0,0));
					//}
					break;
			}
			break;
		case sf::Event::KeyReleased: 
			switch(event.Key.Code){
				case sf::Key::Left:
					cout << "LEFT RELEASED!" << endl;
					roll(-30.0);
					
					break;
				case sf::Key::Right:
					cout << "RIGHT RELEASED!" << endl;
					roll(30.0);
					break;
			}
		default:
			break;
	}
}