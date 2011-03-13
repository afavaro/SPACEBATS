#ifndef BODY_EMITTER_H
#define BODY_EMITTER_H

#include "Framework.h"
#include "Body.h"
#include "Model.h"
#include <list>
#include "btBulletDynamicsCommon.h"

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
		int counter;
		struct ContactCallback : public btCollisionWorld::ContactResultCallback {
			BodyEmitter *bodyEmitter;

			ContactCallback(BodyEmitter *bodyEmitter);
			btScalar addSingleResult(btManifoldPoint &cp,
					const btCollisionObject *colObj0, int partId0, int index0,
					const btCollisionObject *colObj1, int partId1, int index1);
		};
		friend struct ContactCallback;
		ContactCallback *contactCallback;

		btVector3 getAngularVelocityForType(BodyType type);
		btVector3 getPositionForType(BodyType type);
		btVector3 getLinearVelocityForType(BodyType type);
		void setSpeed(float speed);
	
	
		btDiscreteDynamicsWorld *world;
		std::list<Body*> bodies;

		float accum;

		Model models[NUM_BODY_TYPES];
		Assimp::Importer importers[NUM_BODY_TYPES];

		btCollisionShape *collisionShapes[NUM_BODY_TYPES];

		btCollisionObject *wall;
		btCollisionShape *wallShape;
	
		bool boostMode;
};

#endif
