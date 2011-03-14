#ifndef GATE_H
#define GATE_H

#include "Framework.h"
#include "Body.h"
#include <btBulletDynamicsCommon.h>
#include "Model.h"

class Gate: public Body{

public:
	Gate(Model *model, btRigidBody::btRigidBodyConstructionInfo &btInfo, BodyType ty);
	~Gate();
	
	static sf::Image* changed;
	static void loadChangeImage();
	
	void setCompleted();
	virtual void draw(RenderPass pass);

	//static Scoreboard* scoreboard;
	//static void setScoreboard(Scoreboard* s);
	
	
private:
	
	bool completed;
};

#endif // Gate.h
