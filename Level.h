#ifndef LEVEL_H
#define LEVEL_H


/* This class represents a level. A level can be read in from a text file. The text file
 * for the level is of the following form.
 *
 * BACKGROUND IMAGE FILE PATH
 * NUM MODELS
 * LIST OF MODELS TO USE (MODEL #)
 * NUM GATES
 * LIST OF GATE TIME, GATE NUM
 *
 *
 * EXAMPLE
 * 
 * models/space2.jpg
 * 2		// 2 models
 * 4			//use model # 4
 * 0			//use model # 0
 * 4		// 4 landmarks
 * 10 0
 * 25 0
 * 40 0
 * 55 0
 *
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
	
	void print();
	
	sf::Image* getSplash();
	
	void renderBackground();
	BodyType firstLandmark();
	
	bool shouldEmitLandmark(float timeElapsed);

	bool hasBG();

	static Shader* bgShader;
	static void loadShaders();
	
	vector<BodyType> levelTypes;
	
	int number();
	
private:
	int level;
	float speed;
	float endTime;
	string splashFile, bgFile;
	
	vector<Landmark> landmarks;

	sf::Image* background;
	sf::Image* splashImage;
};


#endif // level.h