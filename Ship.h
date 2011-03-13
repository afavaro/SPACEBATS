/*
 *  Ship.h
 *  SPACEBATS
 *
 *  Created by D! on 3/8/11.
 *  Copyright 2011 Stanford University. All rights reserved.
 */

#ifndef SHIP_H
#define SHIP_H

#include "Framework.h"
#include "InputListener.h"
#include "Model.h"
#include "Camera.h"

#include "btBulletDynamicsCommon.h"

#include <cmath>

struct Rotation {
	btQuaternion start;
	btQuaternion end;
	GLfloat time, duration;
};

class Ship : public InputListener {
public:
	Ship(btVector3 pos, Camera* c);
	~Ship();
	
	void handleEvent(sf::Event &event, const sf::Input &input);
	void update(float tstep);
	
	Rotation *curRot;
	Model model;

	btVector3 pos;
	btVector3 velocity;
	btVector3 acceleration;
	btQuaternion quat;

	bool isStopping;
	
	void setWorld(btDiscreteDynamicsWorld* world);
	void testCollision();
	
	btCollisionShape* spaceshipShape; 
	btCollisionObject* spaceshipCollider;	
	
	
	Camera* cam;	

private:
	
	struct ShipContactCallback : public btCollisionWorld::ContactResultCallback {
		Ship* spaceship;
		
		ShipContactCallback(Ship* ship);
		virtual btScalar addSingleResult(btManifoldPoint & cp,
			const btCollisionObject* colObj0, int partId0, int index0,
			const btCollisionObject* colObj1, int partId1, int index1);
	};
	
	friend struct ShipContactCallback;
	
	btCollisionObject* lastCollision;
	
	btDiscreteDynamicsWorld *world;
	ShipContactCallback* callback;
	
	void updatePosition(float tstep);
	void updateRotation(float tstep);

	void setRotation(btQuaternion end);

	btQuaternion neutral;
	btQuaternion maxRollLeft;
	btQuaternion maxRollRight;
	btQuaternion maxPitchUp;
	btQuaternion maxPitchDown;

	bool boostMode;

};

#endif
