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

enum EmitterType {
	SMOKE,
	FIRE,
	PLASMA,
	NUM_EMITTER_TYPES
};

class ParticleEmitter{
public:
	ParticleEmitter(btVector3* pos,  Shader* particleShader, sf::Image* tex, 
					int width, bool fast, EmitterType ty);
	void updateEmitter(float tstep, bool fast);
	void spawnParticles(); 
	void renderParticles(bool fast);
	
private:
	
	int chooseZOffset();
	vector<Particle> particles;
	
	btVector3* position;
	
	sf::Image* particleImage;
	Shader* particleShader;
	
	int screenWidth;
	bool onlyWhenFast;
	EmitterType type;
};

#endif

