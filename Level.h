#ifndef LEVEL_H
#define LEVEL_H


/* This class represents a level. A level can be read in from a text file. The text file
 * for the level is of the following form.
 *
 * BACKGROUND IMAGE FILE PATH
 * GATE LIST
 * X1, Y1, Z1
 * X2, Y2, Z2
 */

#include "Framework.h"
#include "Shader.h"

using namespace std;

class Level{

public:
	Level(int level);
	~Level();
	
	void renderBackground();
	
	static Shader* bgShader;
	static void loadShaders();
	
private:
	int level;
	
	sf::Image* background;
};


#endif // level.h