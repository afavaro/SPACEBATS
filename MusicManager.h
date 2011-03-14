


#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include "Framework.h"
#include <SFML/Audio.hpp>
#include <string>
using namespace std;

enum SoundType {
	POWERUP, 
	SPACESHIP, 
	BACKGROUND, 
	NUM_SOUNDS
};


class MusicManager {

public:
	MusicManager();
	~MusicManager();
	void playSound(SoundType sound);
	
private:
	
	void openFile(SoundType sound, string file);
	sf::Music sounds[NUM_SOUNDS];
};


#endif // music manager.h