#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "HUDComponent.h"
#include "Framework.h"
#include "Shader.h"



class Scoreboard : public HUDComponent{

public:
	Scoreboard(sf::RenderWindow* window, Shader* shader);
	~Scoreboard();
	void render();
	
	void setXLocation(float x);
	
	void setScore(float s);
	void add(float s);
	void subtract(float s);
	void print();
	
	static const float MAX_SCORE;

	float score;
private:
	float xLoc;
	sf::RenderWindow* window;
	Shader* shader;
};

#endif // scoreboard.h