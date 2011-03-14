#ifndef PARTICLE_ENGINE_H
#define PARTICLE_ENGINE_H

#include "Framework.h"
#include "ParticleEmitter.h"
#include "btBulletDynamicsCommon.h"
#include "Shader.h"
#include "Ship.h"
#include <string>
#include <vector>

using namespace std;

class ParticleEngine{
public:
	ParticleEngine(int width); 
	//void addEmitter(Ship* ship, int particleType);
	void addEmitter(btVector3* pos, int particleType);
	void updateEmitters(double tstep, bool fast); 
	void renderEmitters(bool fast);
private:
	int width;
	vector<ParticleEmitter> emitters;
	vector<Shader*> particleShaders;
	vector<sf::Image*> particleImages;
	int counter;
};

#endif

