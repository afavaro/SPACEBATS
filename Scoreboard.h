#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "HUDComponent.h"
#include "Framework.h"

class Scoreboard : public HUDComponent{

public:
	Scoreboard(sf::RenderWindow* window);
	~Scoreboard();
	void render();
	
	void setScore(int s);
private:
	int score;
	sf::RenderWindow* window;
};

#endif // scoreboard.h