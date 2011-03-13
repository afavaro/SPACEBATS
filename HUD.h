#ifndef HUD_H
#define HUD_H

#include "Ship.h"
#include <vector>
#include "HUDComponent.h"


using namespace std;

class HUD{

public:
	HUD(Ship* ship);
	~HUD();
	
	
	void render();
	void addComponent(HUDComponent* component);

	
private:
	
	vector<HUDComponent*> components;
	Ship* ship;
	
	
};

#endif // hud.h