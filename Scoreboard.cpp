#include "Scoreboard.h"
#include "Framework.h"

#include <cstdio>

const float Scoreboard::MAX_SCORE = 200;

const float MAX_HEIGHT = 300;

Scoreboard::Scoreboard(sf::RenderWindow* window, Shader* shader){
	this->window = window;
	this->shader = shader;
	xLoc = 10;
}


Scoreboard::~Scoreboard(){

}

void Scoreboard::setXLocation(float x){
	xLoc = x;
}



void Scoreboard::render(){
//	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//	sf::String scoreString("50", sf::Font::GetDefaultFont(), 30.f);
//	scoreString.SetColor(sf::Color(200, 0,0));
//	scoreString.SetPosition(50.f, 580.f);
//	window->Draw(scoreString);
	
	
	float height = MAX_HEIGHT * score / MAX_SCORE;
	if(height < 5) height = 5;
	if(height > MAX_HEIGHT) height = MAX_HEIGHT;
	glViewport(xLoc,0,50,height);
	
	glUseProgram(shader->programID());
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		
	GLint pos = glGetAttribLocation(shader->programID(), "positionIn");
	glBegin(GL_QUADS);
	glVertexAttrib2f(pos, -1.0, -1.0);
	glVertexAttrib2f(pos, 1.0, -1.0);
	glVertexAttrib2f(pos, 1.0, 1.0);
	glVertexAttrib2f(pos, -1.0, 1.0);
	glEnd();	
}

void Scoreboard::print(){
	printf("%f\n", score);
}

void Scoreboard::setScore(float s){
	score = s;
}

void Scoreboard::add(float s){
	score += s;
	if(score > MAX_SCORE) score = MAX_SCORE;
}

void Scoreboard::subtract(float s){
	score -= s;
	if(score < 0 ) score = 0;
}
