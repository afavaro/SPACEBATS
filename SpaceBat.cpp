
#include "SpaceBat.h"
#include "Framework.h"
#include "Model.h"
#include <iostream>
#include <cmath>

using namespace sf;


sf::Image* SpaceBat::boost;
sf::Image* SpaceBat::normal;
bool SpaceBat::canEat;

SpaceBat::SpaceBat(Model *model, btRigidBody::btRigidBodyConstructionInfo &btInfo, BodyType ty, ParticleEngine* pEngine)

: Body(model, btInfo, ty){
	particleEngine = pEngine;
	canEat = false;
}

void SpaceBat::loadImages(){
	boost = new Image();
	boost->LoadFromFile("models/spacebat/boost.jpg");

	normal = new Image();
	normal->LoadFromFile("models/spacebat/diffuse.jpg");
}

SpaceBat::~SpaceBat() {}

void SpaceBat::setEatable(bool eatable){
	canEat = eatable;
}

void SpaceBat::draw(RenderPass pass){
	btTransform transform;
	this->getMotionState()->getWorldTransform(transform);
	
	model->setTransformation(transform);
	
	if(canEat){
		model->setDiffuseImage(boost);
	}else{
		model->setDiffuseImage(normal);
	}
	model->render(pass);	
}
