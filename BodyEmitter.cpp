
#include "BodyEmitter.h"
#include "Camera.h"
#include "Gate.h"
#include "Ship.h"
#include "SpaceBat.h"

#define BOUNDARY_X 50.0
#define BOUNDARY_Y 44.0
#define BOUNDARY_Z -1000.0

const int BODIES_TO_EMIT = 15;

using namespace std;

BodyEmitter::BodyEmitter(btDiscreteDynamicsWorld *world) {
	this->world = world;
	accum = 0.0;
	boostMode = false;
	
	wallShape = new btStaticPlaneShape(btVector3(0, 0, -1), -80);
	wall = new btCollisionObject();
	wall->setCollisionShape(wallShape);
	
	contactCallback = new ContactCallback(this);
	
	emitSpeed = 1;
}

BodyEmitter::~BodyEmitter() {	
	delete wall;
	delete wallShape;
	delete contactCallback;
}

void BodyEmitter::emit(BodyType type, ParticleEngine* pEngine){
	
	btVector3 pos = getPositionForType(type);
	
	btDefaultMotionState *motionState =
	new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));
	
	btScalar mass = getMassForType(type);
	
	btRigidBody::btRigidBodyConstructionInfo
	constructionInfo(mass, motionState, models[type].getCollisionShape());
	
	Body* newBody;
	
	if(type == GATE){
		newBody = new Gate(&models[type], constructionInfo, type, pEngine );
	}else if(type == SPACEBAT){
		newBody = new SpaceBat(&models[type], constructionInfo, type, pEngine);
	}else{
		newBody = new Body(&models[type], constructionInfo, type);
	}
	newBody->setLinearVelocity(getLinearVelocityForType(type));
	newBody->setAngularVelocity(getAngularVelocityForType(type));
	
	world->addRigidBody(newBody);
	bodies.push_back(newBody);
}


void BodyEmitter::clear(){
	bodies.clear();
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
		case PEPSI:
			return btVector3(RandomFloat(2,4), RandomFloat(1,2), RandomFloat(2,4));
			//case MARS:
			//case APPLE:
			//	return btVector3(0,1,0);
		case GATE:
		case SPACEBAT:
		case JUPITER:
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
		case JUPITER:
		case END:
			return btVector3(0,0, BOUNDARY_Z);
		case GATE:
			//case APPLE:
		case PEPSI:
			return btVector3(
							 ((float)rand() / RAND_MAX * 2.0 * BOUNDARY_X - BOUNDARY_X) / 2.0,
							 ((float)rand() / RAND_MAX * 2.0 * BOUNDARY_Y - BOUNDARY_Y) / 2.0,
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
		case JUPITER:
			return btVector3(0,0, speed);
			//case VENUS:
			//case APPLE:
		case PEPSI:
			return btVector3(0,0, 25);
		case LUSH:
			return btVector3(0.2,0.02, 0.03);
		default:
			//	return btVector3(RandomFloat(-1,1),RandomFloat(-1,1),speed);
			return btVector3(0,0, speed);
	}
}


btScalar BodyEmitter::getMassForType(BodyType type){
	switch(type){
		case GATE:
			return 10;
			//case VENUS:
		case LUSH:
			return 1000;
		case END:
			return 10000;
		default:
			return 4;
	}
}

void BodyEmitter::setEmitSpeed(float speed){
	emitSpeed = speed;
}


void BodyEmitter::emitBodies(float tstep, ParticleEngine* pEngine, Level* level) {
	
	accum += tstep;
	world->contactTest(wall, *contactCallback);
	
	if(emitSpeed == 0) return;
	
	if (accum > emitSpeed) {
		accum = 0.0;
		
		if(level->levelTypes.size() > 0){
			for(int i = 0; i < level->count; i++){
				int index = rand() % level->levelTypes.size();
				BodyType type = level->levelTypes[index];
				emit(type, NULL);
			}
		}
		
		if(level->hasGates)
			emit(GATE, pEngine);
		
	}
}

void BodyEmitter::drawBodies(RenderPass pass) {
	list<Body*>::iterator it;
	for(it = bodies.begin(); it != bodies.end(); it++)
		(*it)->draw(pass);
}

void BodyEmitter::loadModels() {
	//models[MARS].loadFromFile("models/mars", "mars.3ds", importers[MARS]);
	models[ASTEROID].loadFromFile("models/aster", "asteroid.3ds", importers[ASTEROID]);
	models[ASTEROID].setScaleFactor(0.4);
	//models[GATE].loadFromFile("models/aster", "roid.obj", importers[GATE]);
	models[EROS].loadFromFile("models/eros", "eros.3ds", importers[EROS]);
	models[EROS].setScaleFactor(0.7);
	models[GOLEVKA].loadFromFile("models/golevka", "golevka.3ds", importers[GOLEVKA]);
	models[GOLEVKA].setScaleFactor(0.05);
	models[JUNO].loadFromFile("models/juno", "juno.3ds", importers[JUNO]);
	models[JUNO].setScaleFactor(0.05);
	
	models[GATE].loadFromFile("models/gayte", "gate.obj", importers[GATE]);
	models[GATE].setScaleFactor(0.5);
	
	models[SPACEBAT].loadFromFile("models/spacebat", "batty.obj", importers[SPACEBAT]);
	models[SPACEBAT].setScaleFactor(0.5);
	
	models[LUSH].loadFromFile("models/lush", "lush.3DS", importers[LUSH]);
	
	models[JUPITER].loadFromFile("models/jupiter", "jupiter.3ds", importers[JUPITER]);
	models[JUPITER].setScaleFactor(50);
	
	models[PEPSI].loadFromFile("models/pepsi", "pepsi.3ds", importers[PEPSI]);
	models[PEPSI].setScaleFactor(4);
	
	models[END].loadFromFile("models/largeroid", "grande.obj", importers[END]);
	models[END].setScaleFactor(8);
	
	models[MINE].loadFromFile("models/mine", "mine.obj", importers[MINE]);
	models[MINE].setScaleFactor(2);
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
