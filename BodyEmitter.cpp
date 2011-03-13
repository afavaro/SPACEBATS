
#include "BodyEmitter.h"
#include "Camera.h"

#define BOUNDARY_X 50.0
#define BOUNDARY_Y 44.0
#define BOUNDARY_Z -250.0

#define EMIT_STEP 0.3

const float NORMAL_SPEED = 100.0;
const float BOOST_SPEED = 200.0;

using namespace std;

BodyEmitter::BodyEmitter(btDiscreteDynamicsWorld *world) {
	this->world = world;
	accum = 0.0;
	boostMode = false;

	collisionShapes[MARS] = new btSphereShape(5);
	collisionShapes[ASTEROID] = new btSphereShape(5);
	collisionShapes[EROS] = new btSphereShape(5);
	collisionShapes[GOLEVKA] = new btSphereShape(5);
	collisionShapes[JUNO] = new btSphereShape(5);
	
	btScalar mass = 4.0;
	btVector3 inertia(0,0,0);
	for(int i = 0; i < NUM_BODY_TYPES; i++){
		btCollisionShape* shape = collisionShapes[i];
		shape->calculateLocalInertia(mass, inertia);
	}

	wallShape = new btStaticPlaneShape(btVector3(0, 0, -1), -20);
	wall = new btCollisionObject();
	wall->setCollisionShape(wallShape);

	contactCallback = new ContactCallback(this);

// These models were good... but a little too big?
}

BodyEmitter::~BodyEmitter() {	
	for(int i = 0; i < NUM_BODY_TYPES; i++){
		delete collisionShapes[i];
	}
	delete wall;
	delete wallShape;
	delete contactCallback;
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
		//case GATE:
//			return btVector3(0,0,0);
		//case SPACEBAT:
//			return btVector3(0,0,0);
		default:
			return btVector3(RandomFloat(-1,1), RandomFloat(-1,1), RandomFloat(-1,1));
	}
	
}

void BodyEmitter::emitBodies(float tstep) {
	accum += tstep;
	world->contactTest(wall, *contactCallback);
	
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
		
		btVector3 pos(
				(float)rand() / RAND_MAX * 2.0 * BOUNDARY_X - BOUNDARY_X,
				(float)rand() / RAND_MAX * 2.0 * BOUNDARY_Y - BOUNDARY_Y,
				BOUNDARY_Z);
		btDefaultMotionState *motionState =
			new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));
		
		int type = rand() % NUM_BODY_TYPES;
		
		btScalar mass = 4.0;
		
		btRigidBody::btRigidBodyConstructionInfo
			constructionInfo(mass, motionState, collisionShapes[type]);
		
		Body* newBody = new Body(&models[type], constructionInfo);
		
		float speed = boostMode ? BOOST_SPEED : NORMAL_SPEED;
		newBody->setLinearVelocity(btVector3(RandomFloat(-1,1),RandomFloat(-1,1),speed));
		newBody->setAngularVelocity(getAngularVelocityForType(BodyType(type)));

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
