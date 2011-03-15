#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include <vector>
#include "Level.h"
#include "InputListener.h"
#include "Framework.h"
using namespace std;


/*
 * This class implements a LevelManager, which manages the different Levels of the game. 
 * The LevelManager works by reading in levels from files contained in the levels/ directory.
 * Each level is named by a text file with the level number, 1,2...etc.
 */
class LevelManager : public InputListener {

public:
	LevelManager(int numLevels);
	~LevelManager();
	
	Level* getLevel(int i);
	Level* current();
	
	void renderSplash();
	
	bool shouldShowSplashScreen();
	void setSplash(bool s);
	
	
	void handleEvent(sf::Event &event, const sf::Input &input);

	bool last();

	
	void nextLevel();
	int currentLevel;
	
private:
	bool splashOn;
	vector<Level*> levels;
	int numLevels;
	
};

#endif // level manager