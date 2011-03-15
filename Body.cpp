#include "Body.h"

Body::Body(Model *model, btRigidBody::btRigidBodyConstructionInfo &btInfo, BodyType ty)
: btRigidBody(btInfo), type(ty) {
	this->model = model;
}

Body::~Body() {
	delete this->getMotionState();
}

void Body::draw(RenderPass pass) {
	btTransform transform;
	this->getMotionState()->getWorldTransform(transform);
	model->setTransformation(transform);
	model->render(pass);
}


void Body::printType(){
	switch (type) {
		//case MARS: printf("MARS\n"); break;
		case ASTEROID: printf("ASTEROID\n"); break;
		case EROS: printf("EROS\n"); break;
		case GOLEVKA: printf("GOLEVKA\n"); break;
		case JUNO: printf("JUNO\n"); break;
		case GATE: printf("GATE\n"); break;
		case SPACEBAT: printf("SPACEBAT\n"); break;
		default: break;
	}
}



BodyType Body::getType(){
	return type;
}


bool Body::isHealthType(){
	switch (type){
		case PEPSI: 
			return true;
		default:
			return false;
	}
}

bool Body::isCollisionType(){
	return !isHealthType() && !isEndType();
}

bool Body::isEndType(){
	return type == END;
}
