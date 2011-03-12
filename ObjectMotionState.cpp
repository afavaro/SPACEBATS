
#include "ObjectMotionState.h"
#include "btBulletDynamicsCommon.h"


ObjectMotionState::ObjectMotionState(const btTransform & initialPosition){
	this->initialPosition = initialPosition;
}

ObjectMotionState::~ObjectMotionState(){

}

void ObjectMotionState::getWorldTransform(btTransform & worldTrans) const{
	worldTrans = initialPosition;
}

void ObjectMotionState::setWorldTransform(btTransform & worldTrans){
	
}