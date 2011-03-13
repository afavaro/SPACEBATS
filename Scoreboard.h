#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "HUDComponent.h"

class Scoreboard : public HUDComponent{

public:
	Scoreboard();
	~Scoreboard();
	void render();
};

#endif // scoreboard.h