#include "LevelManager.h"

LevelManager::LevelManager(int numLevels){
	this->numLevels = numLevels;
	this->currentLevel = 0;
	for(int i = 0; i < numLevels; i++){
		levels.push_back(new Level(i));
		levels[i]->print();
	}
	
	splashOn = true;
}

LevelManager::~LevelManager(){

}

Level* LevelManager::getLevel(int i){
	return levels[i];
}


Level* LevelManager::current(){
	return levels[currentLevel];
}




void LevelManager::renderSplash(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(Level::bgShader->programID());
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	GLint tex = glGetUniformLocation(Level::bgShader->programID(), "texture");
	glUniform1i(tex, 0);
	glActiveTexture(GL_TEXTURE0);
	//splash.Bind();
	current()->getSplash()->Bind();
	
	GLint pos = glGetAttribLocation(Level::bgShader->programID(), "positionIn");
	glBegin(GL_QUADS);
	glVertexAttrib2f(pos, 1.0, -1.0);
	glVertexAttrib2f(pos, 1.0, 1.0);
	glVertexAttrib2f(pos, -1.0, 1.0);
	glVertexAttrib2f(pos, -1.0, -1.0);
	glEnd();
}

bool LevelManager::shouldShowSplashScreen(){
	return splashOn;
}

void LevelManager::setSplash(bool s){
	splashOn = s;
}


void LevelManager::nextLevel(){
	currentLevel++;
	currentLevel = currentLevel % numLevels;
	setSplash(true);
}

void LevelManager::handleEvent(sf::Event &event, const sf::Input &input) {
	switch (event.Type) {
		case sf::Event::KeyPressed: 
			switch(event.Key.Code){
				case sf::Key::L:
					nextLevel();
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
