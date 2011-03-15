#ifndef GATE_H
#define GATE_H

#include "Framework.h"
#include "ParticleEngine.h"
#include "Body.h"
#include <btBulletDynamicsCommon.h>
#include "Model.h"

class Gate: public Body{

public:
	Gate(Model *model, btRigidBody::btRigidBodyConstructionInfo &btInfo, BodyType ty, ParticleEngine* pEngine);
	~Gate();
	
	static sf::Image* changed;
	static void loadChangeImage();
	
	void setCompleted();
	virtual void draw(RenderPass pass);

	void setRadius(float r);
	float getRadius();
	
private:	
	bool completed;
	ParticleEngine* particleEngine;
	float radius;
};

#endif // Gate.h
