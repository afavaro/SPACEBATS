
#include "Ship.h"
#include <iostream>
#include <math.h>
#include "Body.h"
#include "Gate.h"

#define DURATION 0.2
#define SHAKE_DURATION 0.5
#define THRUST 150.0
#define DRAG 200.0

#define EASE(t) ((t) * (t) * (3.0 - 2.0 * (t)))

#define PITCH_ROTATION	M_PI/15.0
#define ROLL_ROTATION	M_PI/6.0

#define BOUNDARY_X 55.0
#define BOUNDARY_Y 40.0
#define DELTA 0.01

#define STOP_THRESHOLD 2

#define FREEZE_DUR 2.0

using namespace std;


Ship::ShipContactCallback::ShipContactCallback(Ship* ship)
: btCollisionWorld::ContactResultCallback(), spaceship(ship)
{
}

void Ship::setBoostBar(StatusBar* s){
	boostBar = s;
}

void Ship::setHealthBar(StatusBar* s){
	healthBar = s;
}

void Ship::setStatusText(StatusText *st) {
	statusText = st;
}


void Ship::shiverMeTimbers(bool goodExplosion){
	
	if(goodExplosion){
		pEngine->addEmitter(&this->pos, EXPLOSION, false, true, 2);	
		return;
	}else{
		music->playSound(CRASH);
		pEngine->addEmitter(&this->pos, EXPLOSION, false, true);	
	}
	
	delete curShake;
	curShake = new Shake();
	
	curShake->position = pos;
	curShake->time = 0.0;
	curShake->duration = SHAKE_DURATION;
	
	
}

void Ship::updateShake(float tstep){
	if (curShake) {
		curShake->time += tstep;
		if (curShake->time > curShake->duration) {
			delete curShake; curShake = NULL;
		}
		else {
			//float t = curRot->time / curRot->duration;
			float randPos = rand()%10*0.135;
			if(rand()%2) randPos *= -1.0; //curRot->start.slerp(curRot->end, EASE(t));
			pos.m_floats[0] = curShake->position.x() + randPos;
			pos.m_floats[1] = curShake->position.y() + randPos;
			pos.m_floats[2] = curShake->position.z() + randPos;
		}
	}
}

btScalar Ship::ShipContactCallback::addSingleResult(btManifoldPoint & cp,
											  const btCollisionObject* colObj0, int partId0, int index0,
											  const btCollisionObject* colObj1, int partId1, int index1){
	
	if(spaceship->lastCollision == colObj0 || 
	   spaceship->lastCollision == colObj1) {
		return 0;
	}
	if(colObj0 == spaceship->spaceshipCollider){
		spaceship->lastCollision = (btCollisionObject*)colObj1;
	}
	
	if(colObj1 == spaceship->spaceshipCollider){
		spaceship->lastCollision = (btCollisionObject*)colObj0;
	
	}
	
	Body* body = (Body*) spaceship->lastCollision;
	body->printType();
	if(body->getType() == GATE){
		spaceship->music->playSound(RING);
		Gate* gate = (Gate*)body;
		gate->setCompleted();
		if (spaceship->boostMode)
			spaceship->statusText->addScore(10);
		else
			spaceship->statusText->addScore(5);
	} else if(body->isHealthType()){
		spaceship->healthBar->add(5);
		spaceship->shiverMeTimbers(true);
	} else if(body->getType() == END || body->getType() == BIGRED || body->getType() == BIGBAT){
		cout << "LevelEnd!\n";
		spaceship->levelManager->nextLevel();
	} else if(body->getType() == SPACEBAT){
		if(spaceship->boostMode){ // this is good, they get points and a good animation
			spaceship->statusText->addScore(15);
			spaceship->shiverMeTimbers(true);

		} else{
			spaceship->healthBar->subtract(20);
			spaceship->shiverMeTimbers(false);
		}
		
	} else {
		spaceship->shiverMeTimbers(false);
		spaceship->healthBar->subtract(10);
	}
	
	return 0;
}

void Ship::testCollision(){
	//printf("Called ship test collision\n");
	spaceshipCollider->setWorldTransform(btTransform(quat, pos));
	spaceshipCollider->setCollisionShape(model.getCollisionShape());
	world->contactTest(spaceshipCollider, *callback);
}


void Ship::setWorld(btDiscreteDynamicsWorld* world){
	this->world = world;
}


Ship::Ship(btVector3 pos, Camera* c, ParticleEngine* pE, LevelManager* lM, MusicManager* mM) {
	btQuaternion adjust(btVector3(1, 0, 0), -M_PI / 2.0);
	neutral = btQuaternion(btVector3(0, 1, 0), -M_PI / 2.0) * adjust;
	maxRollLeft = btQuaternion(btVector3(0, 0, -1), -ROLL_ROTATION);
	maxRollRight = btQuaternion(btVector3(0, 0, -1), ROLL_ROTATION);
	maxPitchUp = btQuaternion(btVector3(1, 0, 0), PITCH_ROTATION);
	maxPitchDown = btQuaternion(btVector3(1, 0, 0), -PITCH_ROTATION);
	this->music = mM;
	this->cam = c;
	this->pos = pos;
	acceleration = velocity = btVector3(0, 0, 0);
	this->pEngine = pE;
	this->levelManager = lM;
	quat = neutral;
	curRot = NULL;
	isStopping = false;
	boostMode = false;
	
	this->freezeBoost = false;
	this->freezeTime = 0.0;
	
	curShake = NULL;
	
	spaceshipCollider = new btCollisionObject();
	spaceshipCollider->setWorldTransform(btTransform(quat, pos));
	
	world = NULL;
	callback = new ShipContactCallback(this);
	lastCollision = NULL;
	
	//burnerPos = btVector3(pos.x(),pos.y(),pos.z());
}

Ship::~Ship() {}

void Ship::updateRotation(float tstep) {
	if (curRot) {
		curRot->time += tstep;
		if (curRot->time > curRot->duration) {
			quat = curRot->end;
			delete curRot; curRot = NULL;
		}
		else {
			float t = curRot->time / curRot->duration;
			quat = curRot->start.slerp(curRot->end, EASE(t));
		}
	}
}

void Ship::updatePosition(float tstep) {
	pos += velocity * tstep;
	velocity += acceleration * tstep;

	if (pos.x() > BOUNDARY_X) {
		pos.setX(BOUNDARY_X - DELTA);
		if (velocity.x() > 0) velocity.setX(0);
		if (acceleration.x() > 0) acceleration.setX(0);
	}
	if (pos.x() < -BOUNDARY_X) {
		pos.setX(-BOUNDARY_X + DELTA);
		if (velocity.x() < 0) velocity.setX(0);
		if (acceleration.x() < 0) acceleration.setX(0);
	}
	if (pos.y() > BOUNDARY_Y) {
		pos.setY(BOUNDARY_Y - DELTA);
		if (velocity.y() > 0) velocity.setY(0);
		if (acceleration.y() > 0) acceleration.setY(0);
	}
	if (pos.y() < -BOUNDARY_Y) {
		pos.setY(-BOUNDARY_Y + DELTA);
		if (velocity.y() < 0) velocity.setY(0);
		if (acceleration.y() < 0) acceleration.setY(0);
	}

	if (isStopping && velocity.length() < STOP_THRESHOLD) {
		velocity = acceleration = btVector3(0, 0, 0);
		isStopping = false;
	}

	cam->setTarget(btVector3(0.1 * pos.x(), 0.1 * pos.y(), pos.z()));
}

void Ship::update(float tstep) {
	updateRotation(tstep);
	updatePosition(tstep);
	updateShake(tstep);
	btTransform transform(quat, pos);
	model.setTransformation(transform);
	
	statusText->updateTime(tstep);
	
	if(freezeBoost){ 
		freezeTime += tstep;
		if(freezeTime > FREEZE_DUR) freezeBoost = false;
	}
	
	if(boostMode){
		if(boostBar->getValue() <= 0.1 && !freezeBoost){
			freezeBoost = true;
			freezeTime = 0.0;
			return;
		}
		if(levelManager->currentLevel>3)boostBar->subtract(1.0);
		else boostBar->subtract(0.5);
	} else if(!freezeBoost){
		boostBar->add(0.1);
	}
}

void Ship::setRotation(btQuaternion rot) {
	delete curRot;
	curRot = new Rotation();
	curRot->start = quat;
	curRot->end = rot * neutral;
	curRot->time = 0.0;
	curRot->duration = DURATION;
}


void Ship::handleEvent(sf::Event &event, const sf::Input &input) {
	switch (event.Type) {
		case sf::Event::KeyPressed: 
			if (boostMode) break;
			switch(event.Key.Code){
				case sf::Key::Space:
					boostMode = true;
					if (velocity.length() > 0) {
						isStopping = true;
						acceleration = -DRAG * velocity.normalized();
					}
					setRotation(btQuaternion(0, 0, 0, 1));
					break;
				case sf::Key::H:
					healthBar->add(100);
					break;
				case sf::Key::A:
					if ((curRot == NULL && quat != maxRollLeft) || curRot->end == neutral)
						setRotation(maxRollLeft);
					isStopping = false;
					acceleration = btVector3(-THRUST, 0, 0);
					break;
				case sf::Key::D:
					if ((curRot == NULL && quat != maxRollRight) || curRot->end == neutral)
						setRotation(maxRollRight);
					isStopping = false;
					acceleration = btVector3(THRUST, 0, 0);
					break;
				case sf::Key::W:
					if ((curRot == NULL && quat != maxPitchUp) || curRot->end == neutral)
						setRotation(maxPitchUp);
					isStopping = false;
					acceleration = btVector3(0, THRUST, 0);
					break;
				case sf::Key::S:
					if ((curRot == NULL && quat != maxPitchDown) || curRot->end == neutral)
						setRotation(maxPitchDown);
					isStopping = false;
					acceleration = btVector3(0, -THRUST, 0);
					break;
				case sf::Key::T:
					pos.m_floats[2] -= 0.3;
					break;
				case sf::Key::B:
					cout << "X: " << pos.m_floats[0] << " Y: " << pos.m_floats[1] << " Z: " << pos.m_floats[2] << endl;
				default:
					break;
			}
			break;
		case sf::Event::KeyReleased: 
			switch(event.Key.Code){
				case sf::Key::Space:
					boostMode = false;
					break;
				case sf::Key::A:
				case sf::Key::S:
				case sf::Key::W:
				case sf::Key::D:
					if (velocity.length() > 0) {
						isStopping = true;
						acceleration = -DRAG * velocity.normalized();
					}
					setRotation(btQuaternion(0, 0, 0, 1));
					break;
				default:
					break;
			}
		default:
			break;
	}
}

aiMesh* Ship::GetMesh(){
	return this->model.scene->mMeshes[0];
}

