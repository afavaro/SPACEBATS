
#ifndef BODY_H
#define BODY_H

#include "Framework.h"
#include "Model.h"
#include "Framebuffer.h"

#include <list>

#include <btBulletDynamicsCommon.h>

const int NUM_LANDMARKS = 1;

enum BodyType {
	ASTEROID,	//0
	EROS,		//1
	GOLEVKA,	//2
	JUNO,		//3
	GATE,		//4
	SPACEBAT,	//5
	PEPSI,		//6
	JUPITER,	//7
	LUSH,		//8
	END,		//9
	MINE,		//10
	BIGRED,		//11
	BIGBAT,		//12
	NUM_BODY_TYPES
};

class Body: public btRigidBody {
	public:
		Body(Model *model, btRigidBody::btRigidBodyConstructionInfo &btInfo, BodyType ty);
		~Body();

		virtual void draw(RenderPass pass);
		BodyType getType();
		void printType();
	
		bool isHealthType();
		bool isCollisionType();
		bool isEndType();	
		

	protected:
		Model *model;
		BodyType type;
};

#endif
