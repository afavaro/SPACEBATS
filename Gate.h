#ifndef GATE_H
#define GATE_H

#include "Framework.h"
#include "Body.h"
#include <btBulletDynamicsCommon.h>


class Gate: public Body{

public:
	Gate(Model *model, btRigidBody::btRigidBodyConstructionInfo &btInfo, BodyType ty);
	~Gate();
	
	static sf::Image* changed;
	void changeColor();
};

#endif // Gate.h