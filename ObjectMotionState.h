#ifndef OBJECT_MOTION_STATE_H
#define OBJECT_MOTION_STATE_H

#include "Framework.h"
#include "Body.h"
#include "Model.h"
#include "btBulletDynamicsCommon.h"

class ObjectMotionState : public btMotionState{

public:
	
	ObjectMotionState(const btTransform & initialPosition);
	virtual ~ObjectMotionState();
	
	virtual void getWorldTransform(btTransform & worldTrans) const;
	virtual void setWorldTransform(btTransform & worldTrans);
	
private:
	
	btTransform initialPosition;
};

#endif //objectmotionstate.h