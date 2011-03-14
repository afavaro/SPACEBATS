#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

enum EmitterType {
	FIRE,
	SMOKE,
	PLASMA,
	EXPLOSION,
	NUM_EMITTER_TYPES
};

#include "Framework.h"
#include "Shader.h"
#include "btBulletDynamicsCommon.h"
#include "Particle.h"
#include "Ship.h"
#include <string>
#include <vector>

using namespace std;

struct explosion{
	GLfloat time, duration;
};

class ParticleEmitter{
public:
	ParticleEmitter(btVector3* pos,  Shader* particleShader, sf::Image* tex, 
					int width, bool fast, EmitterType ty);
	void updateEmitter(float tstep, bool fast);
	void spawnParticles(); 
	void renderParticles(bool fast);
	float age;
	EmitterType type;
private:
	void startExplosion();
	void updateExplosion(float tstep);
	void randomizeVelocities(float& x, float& y, float& z);
	
	int chooseZOffset();
	vector<Particle> particles;
	
	btVector3* position;
	
	sf::Image* particleImage;
	Shader* particleShader;
	
	int screenWidth;
	bool onlyWhenFast;
	
	explosion curExplosion;
	
};

#endif

