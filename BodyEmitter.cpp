
#include "BodyEmitter.h"

BodyEmitter::BodyEmitter(btDiscreteDynamicsWorld *world) {
	this->world = world;
}

BodyEmitter::~BodyEmitter() {}

void BodyEmitter::emitBodies(float tstep) {
	//TODO: randomly emit bodies
}

void BodyEmitter::drawBodies(RenderPass pass) {
	for(unsigned i = 0; i < bodies.size(); i++)
		bodies[i].draw(pass);
}

void BodyEmitter::loadModels() {
	models[MARS].loadFromFile("models/mars", "mars.3ds", importers[MARS]);
}
