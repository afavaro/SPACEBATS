#include "Scoreboard.h"
#include "Framework.h"

Scoreboard::Scoreboard(sf::RenderWindow* window, Shader* shader){
	this->window = window;
	this->shader = shader;
}


Scoreboard::~Scoreboard(){

}


void Scoreboard::render(){
//	glClear(GL_DEPTH_BUFFER_BIT );
//	glDisable(GL_LIGHTING);
//	glDisable(GL_TEXTURE_2D);
//	
//	glUseProgram(0);
//	glViewport(0,0,300,60);
//	
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	
//	gluOrtho2D(0.0,1.0,0.0,1.0);
	
	
//	glColor4f(1.0, 0.0, 0.0, 1.0);
//	
//	glBegin(GL_QUADS);
//	glVertex2f(0.0,0.0);
//	glVertex2f(0.0,1.0);
//	glVertex2f(1.0,1.0);
//	glVertex2f(1.0,0.0);
//	glEnd();

	
	
//	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//	sf::String scoreString("50", sf::Font::GetDefaultFont(), 30.f);
//	scoreString.SetColor(sf::Color(200, 0,0));
//	scoreString.SetPosition(50.f, 580.f);
//	window->Draw(scoreString);
	
	glViewport(0,0, 100,100);
	
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
	printf("Score: %f\n", score);
}

void Scoreboard::setScore(float s){
	score = s;
}

void Scoreboard::add(float s){
	score += s;
}

void Scoreboard::subtract(float s){
	score -= s;
}