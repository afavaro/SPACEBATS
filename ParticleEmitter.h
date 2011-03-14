#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "Framework.h"
#include "Shader.h"
#include "btBulletDynamicsCommon.h"
#include "Particle.h"
#include "Ship.h"
#include <string>
#include <vector>

using namespace std;

class ParticleEmitter{
public:
	//ParticleEmitter(Ship* ship, Shader* particleShader, sf::Image* tex);
	ParticleEmitter(btVector3* pos,  Shader* particleShader, sf::Image* tex, int width);
	void updateEmitter(float tstep, bool fast);
	void spawnParticles(); 
	void renderParticles();
	
	//Class Vars
	vector<Particle> particles;
	btVector3* position;
	//Ship* spaceship;
	//int type;
	sf::Image* particleImage;
	Shader* particleShader;
	int screenWidth;
};

#endif

