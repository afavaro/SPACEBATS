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
	
	void setScore(float s);
	void add(float s);
	void subtract(float s);
	void print();
	
private:
	float score;
	sf::RenderWindow* window;
	Shader* shader;
};

#endif // scoreboard.h