#include "Level.h"
#include <iostream>
#include <fstream>

Shader* Level::bgShader;



Level::Level(int level){
	this->level = level;
	
	char filename[10];
	sprintf(filename, "levels/%d", level);
	
	ifstream levelFile(filename);
	string dummy;
	///READ SPLASH
	levelFile >> dummy;
	levelFile >> splashFile; 
	
	splashImage = new sf::Image();
	splashImage->LoadFromFile(splashFile);
	
	///READ BG
	levelFile >> dummy;
	levelFile >> bgFile;
	
	if(bgFile == "NONE"){
		background = NULL;
	}else{
		background = new sf::Image();
		background->LoadFromFile(bgFile);
	}
	///READ SPEED
	levelFile >> dummy;
	levelFile >> speed;
	
	//READ ENDTIME
	levelFile >> dummy;
	levelFile >> endTime;
	
	///READ MODEL TYPES
	levelFile >> dummy;
	
	int numTypes;
	levelFile >> numTypes;
	
	int type;
	for(int i = 0; i < numTypes; i++){
		levelFile >> type;
		levelTypes.push_back(BodyType(type));
		
	}
	
	//READ LANDMARKS
	levelFile >> dummy;
	
	int numModels;
	levelFile >> numModels;
	
	float time;
	int landmark;
	for(int i = 0; i < numModels; i++){
		levelFile >> time;
		levelFile >> landmark;
		
		Landmark lm = {time, BodyType(landmark)};
		//lm.print();
		landmarks.push_back(lm);
	}
	
	levelFile.close();
}



Level::~Level(){

}


void Level::print(){
	cout << "============" << endl;
	cout << "Level " << level << endl;
	cout << "Speed: " << speed << endl;
	cout << "EndTime: " << endTime << endl;
	cout << "Splash: " << splashFile << endl;
	cout << "BG: " << bgFile << endl;
	for(unsigned i = 0; i < levelTypes.size(); i++){
		cout << "ModelType #" << levelTypes[i] << endl;
	}
	
	for(unsigned i = 0; i < landmarks.size(); i++){
		cout << "Landmark: ";
		landmarks[i].print();
	}
	
	cout << "============" << endl;
	
}



BodyType Level::firstLandmark(){
	BodyType saved = landmarks[0].type;
	landmarks.erase(landmarks.begin());
	return saved;
}

bool Level::shouldEmitLandmark(float timeElapsed){
	if(landmarks.size() == 0) return false;
	return timeElapsed > landmarks[0].time;
}


sf::Image* Level::getSplash(){
	return splashImage;
}
	

void Level::loadShaders(){
	bgShader = new Shader("shaders/background");
}

void Level::renderBackground(){
	glUseProgram(bgShader->programID());
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	GLint tex = glGetUniformLocation(bgShader->programID(), "texture");
	glUniform1i(tex, 0);
	glActiveTexture(GL_TEXTURE0);
	background->Bind();
	
	GLint pos = glGetAttribLocation(bgShader->programID(), "positionIn");
	glBegin(GL_QUADS);
	glVertexAttrib2f(pos, -1.0, -1.0);
	glVertexAttrib2f(pos, 1.0, -1.0);
	glVertexAttrib2f(pos, 1.0, 1.0);
	glVertexAttrib2f(pos, -1.0, 1.0);
	glEnd();
}
