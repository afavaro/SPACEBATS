
#include "Body.h"

Body::Body(Model *model, btRigidBody::btRigidBodyConstructionInfo &btInfo) {
	this->model = model;
	this->rigidBody = new btRigidBody(btInfo);
}

Body::~Body() {
	delete rigidBody;
}

void Body::draw(RenderPass pass) {
	btTransform transform;
	rigidBody->getMotionState()->getWorldTransform(transform);
	model->setTransformation(transform);
	model->render(pass);
}
