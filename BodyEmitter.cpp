
#include "BodyEmitter.h"
#include "Camera.h"
#include "Gate.h"
#include "Ship.h"

#define BOUNDARY_X 50.0
#define BOUNDARY_Y 44.0
#define BOUNDARY_Z -250.0

#define EMIT_STEP 0.25

using namespace std;

BodyEmitter::BodyEmitter(btDiscreteDynamicsWorld *world) {
	this->world = world;
	accum = 0.0;
	boostMode = false;
	
	wallShape = new btStaticPlaneShape(btVector3(0, 0, -1), -80);
	wall = new btCollisionObject();
	wall->setCollisionShape(wallShape);

	contactCallback = new ContactCallback(this);

// These models were good... but a little too big?
}

BodyEmitter::~BodyEmitter() {	
	delete wall;
	delete wallShape;
	delete contactCallback;
}

void BodyEmitter::emit(BodyType type){
	printf("Now emitting body type %d\n", type);
	
	btVector3 pos = getPositionForType(type);
	btDefaultMotionState *motionState =
	new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));

	btScalar mass = getMassForType(type);
	
	btRigidBody::btRigidBodyConstructionInfo
	constructionInfo(mass, motionState, models[type].getCollisionShape());

	Body* newBody;
	
	if(type == GATE){
		newBody = new Gate(&models[type], constructionInfo, type);
	}else{
		newBody = new Body(&models[type], constructionInfo, type);
	}
	newBody->setLinearVelocity(getLinearVelocityForType(type));
	newBody->setAngularVelocity(getAngularVelocityForType(type));
	
	world->addRigidBody(newBody);
	bodies.push_back(newBody);
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
		case GATE:
		case SPACEBAT:
			return btVector3(0,0,0);
		//case VENUS:
		case LUSH:
			return btVector3(0,0.1,0);
		default:
			return btVector3(RandomFloat(-1,1), RandomFloat(-1,1), RandomFloat(-1,1));
	}
	
}

btVector3 BodyEmitter::getPositionForType(BodyType type){
	switch (type) {
		case GATE:
			return btVector3(
							 ((float)rand() / RAND_MAX * 2.0 * BOUNDARY_X - BOUNDARY_X) / 4.0,
							 ((float)rand() / RAND_MAX * 2.0 * BOUNDARY_Y - BOUNDARY_Y) / 4.0,
							 BOUNDARY_Z );
		default:
			return btVector3(
							 (float)rand() / RAND_MAX * 2.0 * BOUNDARY_X - BOUNDARY_X,
							 (float)rand() / RAND_MAX * 2.0 * BOUNDARY_Y - BOUNDARY_Y,
							 BOUNDARY_Z );
	}
}


btVector3 BodyEmitter::getLinearVelocityForType(BodyType type){
	float speed = boostMode ? BOOST_SPEED : NORMAL_SPEED;
	switch(type){
		case GATE:
			return btVector3(0,0, 20);
		//case VENUS:
		case LUSH:
			return btVector3(0.2,0.02, 0.03);
		default:
			return btVector3(RandomFloat(-1,1),RandomFloat(-1,1),speed);
	}
}


btScalar BodyEmitter::getMassForType(BodyType type){
	switch(type){
		case GATE:
			return 10;
		//case VENUS:
		case LUSH:
			return 1000;
		default:
			return 4;
	}
}


void BodyEmitter::emitBodies(float tstep) {
	accum += tstep;
	world->contactTest(wall, *contactCallback);
	
	if (accum > EMIT_STEP) {
		accum = 0.0;
		

		BodyType type = BodyType(rand() % (NUM_BODY_TYPES - NUM_LANDMARKS));	

		
		btVector3 pos = getPositionForType(type);
		btDefaultMotionState *motionState =
			new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));

		
		btScalar mass = getMassForType(type);
		
		btRigidBody::btRigidBodyConstructionInfo
			constructionInfo(mass, motionState, models[type].getCollisionShape());
		
		
		Body* newBody;
		
		if(type == GATE){
			newBody = new Gate(&models[type], constructionInfo, type);
		}else{
			newBody = new Body(&models[type], constructionInfo, type);
		}
		newBody->setLinearVelocity(getLinearVelocityForType(type));
		newBody->setAngularVelocity(getAngularVelocityForType(type));

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
	models[ASTEROID].setScaleFactor(0.1);
	//models[GATE].loadFromFile("models/aster", "roid.obj", importers[GATE]);
	models[EROS].loadFromFile("models/eros", "eros.3ds", importers[EROS]);
	models[EROS].setScaleFactor(0.7);
	models[GOLEVKA].loadFromFile("models/golevka", "golevka.3ds", importers[GOLEVKA]);
	models[GOLEVKA].setScaleFactor(0.05);
	models[JUNO].loadFromFile("models/juno", "juno.3ds", importers[JUNO]);
	models[JUNO].setScaleFactor(0.05);
	
	
	models[GATE].loadFromFile("models/gayte", "gate.obj", importers[GATE]);
	
	models[SPACEBAT].loadFromFile("models/spacebat", "spacebat.obj", importers[SPACEBAT]);
	models[SPACEBAT].setScaleFactor(0.5);
	
	
	//models[VENUS].loadFromFile("models/venus", "venus.3ds", importers[VENUS]);
	//models[VENUS].setScaleFactor(0.5);
	
	models[LUSH].loadFromFile("models/lush", "lush.3DS", importers[LUSH]);

}

BodyEmitter::ContactCallback::ContactCallback(BodyEmitter *bodyEmitter)
	: btCollisionWorld::ContactResultCallback(), bodyEmitter(bodyEmitter) {}

btScalar BodyEmitter::ContactCallback::addSingleResult(btManifoldPoint &cp,
		const btCollisionObject *colObj0, int partId0, int index0,
		const btCollisionObject *colObj1, int partId1, int index1) {
	Body *body = (colObj0 == bodyEmitter->wall)? (Body *)colObj1 : (Body *)colObj0;
	bodyEmitter->world->removeRigidBody(body);
	list<Body*>::iterator it;
	for (it = bodyEmitter->bodies.begin(); it != bodyEmitter->bodies.end(); it++) {
		if (*it == body) {
			bodyEmitter->bodies.erase(it);
			delete body;
			break;
		}
	}
	return 0.0;
}
