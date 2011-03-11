
#include "Body.h"

Body::Body(Model *model, btRigidBody::btRigidBodyConstructionInfo &btInfo)
: btRigidBody(btInfo) {
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
