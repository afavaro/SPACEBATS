#ifndef BODY_EMITTER_H
#define BODY_EMITTER_H

#include "Framework.h"
#include "Body.h"
#include "Model.h"

#include <list>

#include "btBulletDynamicsCommon.h"

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

	private:
		btDiscreteDynamicsWorld *world;
		std::list<Body*> bodies;

		float accum;

		Model models[NUM_BODY_TYPES];
		Assimp::Importer importers[NUM_BODY_TYPES];

		btCollisionShape *collisionShapes[NUM_BODY_TYPES];
};

#endif
