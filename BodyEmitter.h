#ifndef BODY_EMITTER_H
#define BODY_EMITTER_H

#include "Framework.h"
#include "Body.h"
#include "Model.h"

#include "btBulletDynamicsCommon.h"

enum BodyType {
	MARS,
	NUM_BODY_TYPES
};

class BodyEmitter {
	public:
		BodyEmitter(btDiscreteDynamicsWorld *world);
		~BodyEmitter();

		void loadModels();

		void emitBodies(float tstep);
		void drawBodies(RenderPass pass);

	private:
		btDiscreteDynamicsWorld *world;
		std::vector<Body> bodies;

		Model models[NUM_BODY_TYPES];
		Assimp::Importer importers[NUM_BODY_TYPES];
};

#endif
