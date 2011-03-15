#ifndef SPACE_BAT_H
#define SPACE_BAT_H

#include "Framework.h"
#include "ParticleEngine.h"
#include "Body.h"
#include <btBulletDynamicsCommon.h>
#include "Model.h"

class SpaceBat: public Body{

public:
	SpaceBat(Model *model, btRigidBody::btRigidBodyConstructionInfo &btInfo, BodyType ty, ParticleEngine* pEngine);
	~SpaceBat();
	
	static sf::Image* boost;
	static sf::Image* normal;
	static void loadImages();
	
	static void setEatable(bool eatable);
	
	virtual void draw(RenderPass pass);
	
	static bool canEat;
	
private:	
	ParticleEngine* particleEngine;
};

#endif // Gate.h
