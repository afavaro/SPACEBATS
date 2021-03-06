/*
 *  Ship.h
 *  SPACEBATS
 *
 *  Created by D! on 3/8/11.
 *  Copyright 2011 Stanford University. All rights reserved.
 */

class ParticleEngine;
class ParticleEmitter;
class LevelManager;

#ifndef SHIP_H
#define SHIP_H



#include "Framework.h"
#include "InputListener.h"
#include "Model.h"
#include "Camera.h"
#include "StatusBar.h"
#include "ParticleEngine.h"
#include "StatusText.h"
#include "btBulletDynamicsCommon.h"
#include "LevelManager.h"
#include "MusicManager.h"
#include <cmath>
#include "Body.h"

using namespace std;

#define NORMAL_SPEED 150.0
#define BOOST_SPEED 250.0

struct Rotation {
	btQuaternion start;
	btQuaternion end;
	GLfloat time, duration;
};

struct Shake {
	btVector3 position;
	GLfloat time, duration;
};

class Ship : public InputListener {
public:
	Ship(btVector3 pos, Camera* c, ParticleEngine* pE, LevelManager* lM, MusicManager* mM);
	~Ship();
	
	void handleEvent(sf::Event &event, const sf::Input &input);
	void update(float tstep);
	
	void setBoostBar(StatusBar* s);
	void setHealthBar(StatusBar* s);
	void setStatusText(StatusText *st);
	
	Rotation *curRot;
	Model model;

	btVector3 pos;
	btVector3 velocity;
	btVector3 acceleration;
	btQuaternion quat;

	bool isStopping;
	
	Camera* cam;
	
	void setWorld(btDiscreteDynamicsWorld* world);
	void testCollision();
	
	btCollisionShape* spaceshipShape; 
	btCollisionObject* spaceshipCollider;	
	
	aiMesh* GetMesh();

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
	
	StatusBar* boostBar;
	StatusBar* healthBar;
	StatusText *statusText;
	
	void updatePosition(float tstep);
	void updateRotation(float tstep);

	void setRotation(btQuaternion end);
	
	void shiverMeTimbers(bool goodExplosion);
	void updateShake(float tstep);
	Shake* curShake;
	
	btQuaternion neutral;
	btQuaternion maxRollLeft;
	btQuaternion maxRollRight;
	btQuaternion maxPitchUp;
	btQuaternion maxPitchDown;

	bool boostMode;
	bool freezeBoost;
	float freezeTime;
	
	ParticleEngine* pEngine;
	LevelManager* levelManager;
	MusicManager* music;
};

#endif
