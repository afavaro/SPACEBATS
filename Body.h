
#ifndef BODY_H
#define BODY_H

#include "Framework.h"
#include "Model.h"
#include "Framebuffer.h"

#include <list>

#include <btBulletDynamicsCommon.h>

const int NUM_LANDMARKS = 1;

enum BodyType {
	ASTEROID,
	EROS,
	GOLEVKA,
	JUNO,
	GATE,
	SPACEBAT,
	PEPSI,
	JUPITER,
	LUSH,
	END,
	//  MINE,
	//	APPLE,
	//	PIZZA,
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
