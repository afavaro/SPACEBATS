
#ifndef BODY_H
#define BODY_H

#include "Framework.h"
#include "Model.h"
#include "Framebuffer.h"

#include <btBulletDynamicsCommon.h>

class Body {
	public:
		Body(Model *model, btRigidBody::btRigidBodyConstructionInfo &btInfo);
		~Body();

		void draw(RenderPass pass);

	private:
		btRigidBody *rigidBody;
		Model *model;
};

#endif
