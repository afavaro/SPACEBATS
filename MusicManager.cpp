

#include "MusicManager.h"

#include <cstdio>


MusicManager::MusicManager(){	
	openFile(BACKGROUND, "music/change.wav");
	openFile(POWERUP, "music/powerup.wav");
	openFile(CRASH, "music/crash.wav");
}


MusicManager::~MusicManager(){

}


void MusicManager::playSound(SoundType sound){
	sounds[sound].Play();
}

void MusicManager::pauseSound(SoundType sound){
	sounds[sound].Pause();
}

void MusicManager::stopSound(SoundType sound){
	sounds[sound].Stop();
}

void MusicManager::loopSound(SoundType sound){
	sounds[sound].SetLoop(true);
}

void MusicManager::openFile(SoundType sound, string file){
	if(!sounds[sound].OpenFromFile(file)){
		printf("Error opening sound file %s.\n", file.c_str());
	}
}
