#ifndef PARTICLE_H
#define PARTICLE_H

#include "Framework.h"
#include "btBulletDynamicsCommon.h"
#include <string>
#include <vector>

using namespace std;

class Particle{
public:
	Particle(btVector3 pos, btVector3 vel);
	void updateParticle(float tstep, bool fast);
	void renderParticle();
	//Class Vars
	btVector3 position; 
	btVector3 velocity;
	GLfloat age;
};

#endif
