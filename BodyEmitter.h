#ifndef BODY_EMITTER_H
#define BODY_EMITTER_H

#include "Framework.h"
#include "Body.h"

#include "btBulletDynamicsCommon.h"

class BodyEmitter {
	public:
		BodyEmitter(btDiscreteDynamicsWorld *world);
		~BodyEmitter();

		void emitBodies(float tstep);
		void drawBodies();

	private:
		btDiscreteDynamicsWorld *world;
		std::vector<Body> bodies;
};

#endif
