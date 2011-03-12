#include "ObjectMotionState.h"
#include "btBulletDynamicsCommon.h"


ObjectMotionState::ObjectMotionState(const btTransform & transform){
	this->transform = transform;
}

ObjectMotionState::~ObjectMotionState(){

}

void ObjectMotionState::getWorldTransform(btTransform & worldTrans) const{
//	printf("gWT\n");
//	btVector3 newOrigin = transform.getOrigin() + btVector3(rand() % 10,rand() % 10,rand() % 10);
//	btQuaternion rot = transform.getRotation();
//	btTransform newTransform(rot, newOrigin);
//	worldTrans = newTransform;
	worldTrans = transform;
}

void ObjectMotionState::setWorldTransform(btTransform & worldTrans){
	transform = worldTrans;
}