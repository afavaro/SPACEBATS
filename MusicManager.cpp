

#include "MusicManager.h"



MusicManager::MusicManager(){	
	openFile(BACKGROUND, "music/change.wav");
	openFile(POWERUP, "music/powerup.wav");
}


MusicManager::~MusicManager(){

}


void MusicManager::playSound(SoundType sound){
	sounds[sound].Play();
}

void MusicManager::openFile(SoundType sound, string file){
	if(!sounds[sound].OpenFromFile(file)){
		printf("Error opening sound file %s.\n", file.c_str());
	}
}
