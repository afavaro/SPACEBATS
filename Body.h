
#ifndef BODY_H
#define BODY_H

#include "Framework.h"
#include "Model.h"

#include <btBulletDynamicsCommon.h>

class Body {
	public:
		Body(Model *model, btRigidBody::btRigidBodyConstructionInfo &btInfo);
		~Body();

		void draw();

	private:
		btRigidBody *rigidBody;
		Model *model;
};

#endif
