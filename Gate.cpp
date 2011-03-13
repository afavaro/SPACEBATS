


#include "Gate.h"
#include "Framework.h"
#include "Model.h"

using namespace sf;


sf::Image* Gate::changed = NULL;

Gate::Gate(Model *model, btRigidBody::btRigidBodyConstructionInfo &btInfo, BodyType ty)
: Body(model, btInfo, ty){
	if(changed == NULL){
		changed->LoadFromFile("green.jpg");
	}
}


Gate::~Gate(){

}

void Gate::changeColor(){
	model->setDiffuseImage(changed);
}