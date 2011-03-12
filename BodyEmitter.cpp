
#include "BodyEmitter.h"
#include "ObjectMotionState.h"

#define BOUNDARY_X 50.0
#define BOUNDARY_Y 44.0
#define BOUNDARY_Z -100.0

#define EMIT_STEP 1.0

using namespace std;

BodyEmitter::BodyEmitter(btDiscreteDynamicsWorld *world) {
	this->world = world;
	accum = 0.0;

	collisionShapes[MARS] = new btSphereShape(1);
	collisionShapes[ASTEROID] = new btSphereShape(1);
	collisionShapes[EROS] = new btSphereShape(1);
	
	btScalar mass = 4.0;
	btVector3 inertia(0,0,0);
	for(int i = 0; i < NUM_BODY_TYPES; i++){
		btCollisionShape* shape = collisionShapes[i];
		shape->calculateLocalInertia(mass, inertia);
	}

// These models were good... but a little too big?
//	collisionShapes[GOLEVKA] = new btSphereShape(1);
//	collisionShapes[JUNO] = new btSphereShape(1);
}

BodyEmitter::~BodyEmitter() {	
	for(int i = 0; i < NUM_BODY_TYPES; i++){
		delete collisionShapes[i];
	}
}

void BodyEmitter::emitBodies(float tstep) {
	accum += tstep;
	if (accum > EMIT_STEP) {
		accum = 0.0;

		if(bodies.size() > 0) return;
		
//		if (bodies.size() > 1) {
//			delete bodies.back();
//			bodies.pop_back();
//		}

		btVector3 pos(
				(float)rand() / RAND_MAX * 2.0 * BOUNDARY_X - BOUNDARY_X,
				(float)rand() / RAND_MAX * 2.0 * BOUNDARY_Y - BOUNDARY_Y,
				-150);
		btDefaultMotionState *motionState =
			new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));
		
		//ObjectMotionState *motionState =
		//	new ObjectMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));
		
		int type = rand() % NUM_BODY_TYPES;
		
		btScalar mass = 4.0;
		
		btRigidBody::btRigidBodyConstructionInfo
			constructionInfo(mass, motionState, collisionShapes[type]);
		
		Body* newBody = new Body(&models[type], constructionInfo);
		newBody->setLinearVelocity(btVector3(10,10,0));
		
		world->addRigidBody(newBody);
		
		bodies.push_front(newBody);
	}
}

void BodyEmitter::drawBodies(RenderPass pass) {
	list<Body*>::iterator it;
	for(it = bodies.begin(); it != bodies.end(); it++)
		(*it)->draw(pass);
}

void BodyEmitter::loadModels() {
	models[MARS].loadFromFile("models/mars", "mars.3ds", importers[MARS]);
	models[ASTEROID].loadFromFile("models/aster", "asteroid.3ds", importers[ASTEROID]);
	models[EROS].loadFromFile("models/eros", "eros.3ds", importers[EROS]);
//	models[GOLEVKA].loadFromFile("models/golevka", "golevka.3ds", importers[GOLEVKA]);
//	models[JUNO].loadFromFile("models/juno", "juno.3ds", importers[JUNO]);
}
