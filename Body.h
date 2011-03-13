
#ifndef BODY_H
#define BODY_H

#include "Framework.h"
#include "Model.h"
#include "Framebuffer.h"

#include <list>

#include <btBulletDynamicsCommon.h>

class Body: public btRigidBody {
	public:
		Body(Model *model, btRigidBody::btRigidBodyConstructionInfo &btInfo);
		~Body();

		void draw(RenderPass pass);

	private:
		Model *model;
};

#endif
