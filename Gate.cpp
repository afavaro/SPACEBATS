


#include "Gate.h"
#include "Framework.h"
#include "Model.h"
#include "Scoreboard.h"

using namespace sf;


sf::Image* Gate::changed;

Scoreboard* Gate::scoreboard;


Gate::Gate(Model *model, btRigidBody::btRigidBodyConstructionInfo &btInfo, BodyType ty)
: Body(model, btInfo, ty){
	
	completed = false;
}

void Gate::loadChangeImage(){
	changed = new Image();
	changed->LoadFromFile("models/green.jpg");
	printf("Loaded gate image\n");
}


Gate::~Gate(){
	
}

void Gate::setScoreboard(Scoreboard* s){
	scoreboard = s;
}


void Gate::setCompleted(){
	completed = true;
	scoreboard->add(10);
	scoreboard->print();
}

void Gate::draw(RenderPass pass){
	btTransform transform;
	this->getMotionState()->getWorldTransform(transform);
	model->setTransformation(transform);
	
	sf::Image* saved = NULL;
	if(completed){
		saved = model->getDiffuseImage();
		model->setDiffuseImage(changed);
	}
	
	model->render(pass);
	
	if(completed){
		model->setDiffuseImage(saved);
	}
}
