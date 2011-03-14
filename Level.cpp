#include "Level.h"
#include <iostream>
#include <fstream>

Shader* Level::bgShader;

Level::Level(int level){
	this->level = level;
	
	char filename[10];
	sprintf(filename, "levels/%d", level);
	
	ifstream levelFile(filename);
	
	string bgFile;
	getline(levelFile, bgFile);
	cout << "bg: " << bgFile << endl;
	
	background = new sf::Image();
	background->LoadFromFile(bgFile);
}

Level::~Level(){

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
