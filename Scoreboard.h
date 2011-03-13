#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "HUDComponent.h"
#include "Framework.h"

class Scoreboard : public HUDComponent{

public:
	Scoreboard(sf::RenderWindow* window);
	~Scoreboard();
	void render();
	
	void setScore(float s);
	void add(float s);
	void subtract(float s);
	void print();
	
private:
	float score;
	sf::RenderWindow* window;
};

#endif // scoreboard.h