
#include "BodyEmitter.h"

#define BOUNDARY_X 50.0
#define BOUNDARY_Y 44.0
#define BOUNDARY_Z -100.0

#define EMIT_STEP 1.0

using namespace std;

BodyEmitter::BodyEmitter(btDiscreteDynamicsWorld *world) {
	this->world = world;
	accum = 0.0;

	collisionShapes[MARS] = new btSphereShape(1);
}

BodyEmitter::~BodyEmitter() {
	delete collisionShapes[MARS];
}

void BodyEmitter::emitBodies(float tstep) {
	accum += tstep;
	if (accum > EMIT_STEP) {
		accum = 0.0;

		if (bodies.size() > 0) {
			world->removeRigidBody(bodies.front());
			delete bodies.front();
			bodies.pop_front();
		}

		btVector3 pos(
				(float)rand() / RAND_MAX * 2.0 * BOUNDARY_X - BOUNDARY_X,
				(float)rand() / RAND_MAX * 2.0 * BOUNDARY_Y - BOUNDARY_Y,
				BOUNDARY_Z);
		btDefaultMotionState *motionState =
			new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));
		btRigidBody::btRigidBodyConstructionInfo
			constructionInfo(0, motionState, collisionShapes[MARS]);
		Body *body = new Body(&models[MARS], constructionInfo);

		world->addRigidBody(body);
		bodies.push_front(body);
		body->iter = bodies.begin();
	}
}

void BodyEmitter::drawBodies(RenderPass pass) {
	list<Body*>::iterator it;
	for(it = bodies.begin(); it != bodies.end(); it++)
		(*it)->draw(pass);
}

void BodyEmitter::loadModels() {
	models[MARS].loadFromFile("models/mars", "mars.3ds", importers[MARS]);
}
