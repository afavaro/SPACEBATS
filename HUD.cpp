#include "HUD.h"
#include "HUDComponent.h"

HUD::HUD(Ship* ship){
	this->ship = ship;
}


HUD::~HUD(){

}


void HUD::render(){
	for(int i = 0; i < (int)components.size(); i++){
		components[i]->render();
	}
}


void HUD::addComponent(HUDComponent* component){
	components.push_back(component);
}

