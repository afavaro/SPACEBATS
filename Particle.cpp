
#include "Particle.h"
#include <iostream>
using namespace std;

Particle::Particle(btVector3 pos, btVector3 vel){
	this->age = 0.0;
	this->position = pos;
	this->velocity = vel;
}

void Particle::updateParticle(float tstep, bool fast){
	GLfloat rate = 0.6;
	if(fast) {
		rate = 1.5;
		position.m_floats[2] += velocity.m_floats[2]*rate;
	}
	position += velocity * rate;
	// * tstep;
	//cout << "Particle: x:" << this->position.x() << " y:" << this->position.y() << " z: "<< this->position.z() << endl;
	age+=0.01;
}