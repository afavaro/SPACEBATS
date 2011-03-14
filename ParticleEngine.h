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
	ParticleEngine();
	void addEmitter(btVector3* pos, EmitterType type, bool onlyActiveWhenFast, bool timed=false);
	void updateEmitters(double tstep, bool fast); 
	void renderEmitters(bool fast);
	void setWindow(int width);
private:
	int width;
	vector<ParticleEmitter> emitters;
	vector<Shader*> particleShaders;
	vector<sf::Image*> particleImages;
	int counter;
};

#endif

