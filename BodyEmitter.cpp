
#include "BodyEmitter.h"
#include "ObjectMotionState.h"
#include "Camera.h"

#define BOUNDARY_X 50.0
#define BOUNDARY_Y 44.0
#define BOUNDARY_Z -100.0

#define EMIT_STEP 1.0

const float NORMAL_SPEED = 5.0;
const float BOOST_SPEED = 105.0;

using namespace std;

BodyEmitter::BodyEmitter(btDiscreteDynamicsWorld *world) {
	this->world = world;
	accum = 0.0;
	boostMode = false;

	collisionShapes[MARS] = new btSphereShape(20);
	collisionShapes[ASTEROID] = new btSphereShape(20);
	collisionShapes[EROS] = new btSphereShape(20);
	
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

void BodyEmitter::setSpeed(float speed){
	list<Body*>::iterator it;
	for(it = bodies.begin(); it != bodies.end(); it++){
		Body* b = *it;
		btVector3 vel = b->getLinearVelocity();
		vel.setZ(speed);
		b->setLinearVelocity(vel);
	}
}

void BodyEmitter::boostSpeed(){
	setSpeed(BOOST_SPEED);
}

void BodyEmitter::resetSpeed(){
	setSpeed(NORMAL_SPEED);
}

void BodyEmitter::setBoostMode(bool boost){
	boostMode = boost;
}


float RandomFloat(float min, float max){
	float t = (float)rand() / RAND_MAX;
	return min + t * (max - min);
}


btVector3 BodyEmitter::getAngularVelocityForType(BodyType type){
	switch (type){
		case MARS:
			return btVector3(0,1,0);
		default:
			return btVector3(RandomFloat(-1,1), RandomFloat(-1,1), RandomFloat(-1,1));
	}
	
}

void BodyEmitter::emitBodies(float tstep) {
	accum += tstep;
	
	list<Body*>::iterator it;
	for(it = bodies.begin(); it != bodies.end(); it++){
		Body* b = (*it);
		btTransform trans;
		b->getMotionState()->getWorldTransform(trans);
		
		//printf("z: %f\n", trans.getOrigin().getZ());
	}
	//printf("--\n");
	
	if (accum > EMIT_STEP) {
		accum = 0.0;
		
		if(bodies.size() > 10) return;

		printf("%d\n", (int)bodies.size());
		
		btVector3 pos(
				(float)rand() / RAND_MAX * 2.0 * BOUNDARY_X - BOUNDARY_X,
				(float)rand() / RAND_MAX * 2.0 * BOUNDARY_Y - BOUNDARY_Y,
				-250);
		btDefaultMotionState *motionState =
			new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));
		
		//ObjectMotionState *motionState =
		//	new ObjectMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));
		
		int type = rand() % NUM_BODY_TYPES;
		
		btScalar mass = 4.0;
		
		btRigidBody::btRigidBodyConstructionInfo
			constructionInfo(mass, motionState, collisionShapes[type]);
		
		Body* newBody = new Body(&models[type], constructionInfo);
		newBody->setLinearVelocity(btVector3(RandomFloat(-4,4),RandomFloat(-4,4),RandomFloat(-10,10)));
		
		float speed = boostMode ? BOOST_SPEED : NORMAL_SPEED;
//		newBody->setLinearVelocity(btVector3(RandomFloat(-4,4),RandomFloat(-4,4),speed));
		newBody->setAngularVelocity(getAngularVelocityForType(BodyType(type)));
		//		newBody->setAngularVelocity(btVector3(RandomFloat(-1,1), RandomFloat(-1,1), RandomFloat(-1,1)));
		world->addRigidBody(newBody);
		bodies.push_back(newBody);
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
