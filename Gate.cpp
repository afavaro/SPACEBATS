


#include "Gate.h"
#include "Framework.h"
#include "Model.h"
#include <iostream>
#include <cmath>

using namespace sf;


sf::Image* Gate::changed;

Gate::Gate(Model *model, btRigidBody::btRigidBodyConstructionInfo &btInfo, BodyType ty, ParticleEngine* pEngine)

: Body(model, btInfo, ty){
	particleEngine = pEngine;
	completed = false;
	radius = 5.0;
}

void Gate::loadChangeImage(){
	changed = new Image();
	changed->LoadFromFile("models/green.jpg");
	printf("Loaded gate image\n");
}


Gate::~Gate() {}

void Gate::setCompleted(){
	completed = true;
	
	btTransform gateTransform;
	this->getMotionState()->getWorldTransform(gateTransform);
	btVector3 gatePos = gateTransform.getOrigin();
	particleEngine->addEmitter(&gatePos, FIRE, false, true, 2);

}

void Gate::draw(RenderPass pass){
	btTransform gateTransform;
	this->getMotionState()->getWorldTransform(gateTransform);
	
	float scaleF = model->scaleFactor;
	if(completed){
		gateTransform*=btTransform(btQuaternion(0,0,0,1),btVector3(0.1,0.1,0.1));
	}
	/*
	for (int i = 0; i < 6; i++) {
		float t = (float)i * M_PI / 3.0;
		btVector3 translation(-radius * sin(t), radius * cos(t), 0);
		translation += gateTransform.getOrigin();

		btQuaternion rotation(btVector3(0, 0, 1), t);
		btTransform transform(rotation, translation);

		model->setTransformation(transform);
		model->render(pass);
	}*/
	model->setTransformation(gateTransform);
	model->render(pass);
	
//	sf::Image* saved = NULL;
//	if(completed){
//		saved = model->getDiffuseImage();
//		model->setDiffuseImage(changed);
//	}
		
	
	/*
	if(completed){
		model->setDiffuseImage(saved);
	}*/
}
