#include "LevelManager.h"

LevelManager::LevelManager(int numLevels){
	this->numLevels = numLevels;
	this->currentLevel = 0;
	for(int i = 0; i < numLevels; i++){
		levels.push_back(new Level(i));
		levels[i]->print();
	}
}

LevelManager::~LevelManager(){

}

Level* LevelManager::getLevel(int i){
	return levels[i];
}


Level* LevelManager::current(){
	return levels[currentLevel];
}


void LevelManager::handleEvent(sf::Event &event, const sf::Input &input) {
	switch (event.Type) {
		case sf::Event::KeyPressed: 
			switch(event.Key.Code){
				case sf::Key::L:
					currentLevel++;
					currentLevel = currentLevel % numLevels;
					printf("Current level is now %d\n", currentLevel);
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}
