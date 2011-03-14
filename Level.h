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
#include "Body.h"
#include <queue>

using namespace std;

struct Landmark{
	float time;
	BodyType type;
	
	void print(){
		printf("T: %f : %d\n", time, type);
	}
};


class Level{

public:
	Level(int level);
	~Level();
	
	void renderBackground();
	BodyType firstLandmark();
	
	bool shouldEmitLandmark(float timeElapsed);
	
	static Shader* bgShader;
	static void loadShaders();
	
private:
	int level;
	
	queue<Landmark> landmarks;
	sf::Image* background;
};


#endif // level.h