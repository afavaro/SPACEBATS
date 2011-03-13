#ifndef BODY_EMITTER_H
#define BODY_EMITTER_H

#include "Framework.h"
#include "Body.h"
#include "Model.h"

#include <list>

#include "btBulletDynamicsCommon.h"
#include "ObjectMotionState.h"

enum BodyType {
	MARS,
	ASTEROID,
	EROS,
	NUM_BODY_TYPES
};

//GOLEVKA,
//JUNO,

class BodyEmitter {
	public:
		BodyEmitter(btDiscreteDynamicsWorld *world);
		~BodyEmitter();

		void loadModels();

		void emitBodies(float tstep);
		void drawBodies(RenderPass pass);
	
		void boostSpeed();
		void resetSpeed();
		void setBoostMode(bool boost);	

	private:
		btVector3 getAngularVelocityForType(BodyType type);

	
	
		btDiscreteDynamicsWorld *world;
		std::list<Body*> bodies;

		float accum;

		Model models[NUM_BODY_TYPES];
		Assimp::Importer importers[NUM_BODY_TYPES];

		btCollisionShape *collisionShapes[NUM_BODY_TYPES];
	
		bool boostMode;
};

#endif
