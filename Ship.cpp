
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

#define BOUNDARY_X 35.0
#define BOUNDARY_Y 25.0
#define DELTA 0.01

#define STOP_THRESHOLD 2

using namespace std;


Ship::ShipContactCallback::ShipContactCallback(Ship* ship)
: btCollisionWorld::ContactResultCallback(), spaceship(ship)
{
}

void Ship::setScoreboard(Scoreboard* s){
	scoreboard = s;
}


void Ship::shiverMeTimbers(){
	//if(!curShake) return;
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
			float randPos = rand()%10*0.1;
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
		Gate* gate = (Gate*)body;
		gate->setCompleted();
		//printf("GATE COMPLETED!\n");
	}else {
		spaceship->shiverMeTimbers();
		
		spaceship->scoreboard->subtract(5);
		spaceship->scoreboard->print();
		printf("COLLISION\n");
	}
	

	return 0;
}

void Ship::testCollision(){
	//printf("Called ship test collision\n");
	spaceshipCollider->setWorldTransform(btTransform(quat, pos));
	world->contactTest(spaceshipCollider, *callback);
}


void Ship::setWorld(btDiscreteDynamicsWorld* world){
	this->world = world;
}


Ship::Ship(btVector3 pos, Camera* c) {
	btQuaternion adjust(btVector3(1, 0, 0), -M_PI / 2.0);
	neutral = btQuaternion(btVector3(0, 1, 0), -M_PI / 2.0) * adjust;
	maxRollLeft = btQuaternion(btVector3(0, 0, -1), -ROLL_ROTATION);
	maxRollRight = btQuaternion(btVector3(0, 0, -1), ROLL_ROTATION);
	maxPitchUp = btQuaternion(btVector3(1, 0, 0), PITCH_ROTATION);
	maxPitchDown = btQuaternion(btVector3(1, 0, 0), -PITCH_ROTATION);

	this->pos = pos;
	acceleration = velocity = btVector3(0, 0, 0);
	this->cam = c;
	quat = neutral;
	curRot = NULL;
	isStopping = false;
	boostMode = false;
	
	curShake = NULL;
	
	spaceshipShape = new btSphereShape(6);
	spaceshipCollider = new btCollisionObject();
	spaceshipCollider->setWorldTransform(btTransform(quat, pos));
	spaceshipCollider->setCollisionShape(spaceshipShape);
	
	world = NULL;
	callback = new ShipContactCallback(this);
	lastCollision = NULL;
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
	
	
	if(boostMode){
		scoreboard->subtract(1);
	}else{
		scoreboard->add(0.05);
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
