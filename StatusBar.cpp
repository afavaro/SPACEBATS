
#include "StatusBar.h"
#include "Framework.h"

#include <cstdio>

#define INSET 0.005

StatusBar::StatusBar(Shader* shader, btVector4 rect){
	this->rect = rect;
	this->shader = shader;
	topColor = bottomColor = btVector4(0.6, 0.81, 0.92, 0.5);
	value = 100;
}

StatusBar::~StatusBar() {}

void StatusBar::render(){
	glEnable(GL_BLEND); //GL_CHECK
	glDepthMask(GL_FALSE); //GL_CHECK
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //GL_CHECK
		
	glUseProgram(shader->programID());
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		
	GLint pos = glGetAttribLocation(shader->programID(), "positionIn");
	GLint col = glGetAttribLocation(shader->programID(), "colorIn");

	glBegin(GL_LINE_LOOP);
	glVertexAttrib4f(col, 0.6, 0.81, 0.92, 0.5);
	glVertexAttrib2f(pos, rect.x(), rect.y());
	glVertexAttrib4f(col, 0.6, 0.81, 0.92, 0.5);
	glVertexAttrib2f(pos, rect.x() + rect.z(), rect.y());
	glVertexAttrib4f(col, 0.6, 0.81, 0.92, 0.5);
	glVertexAttrib2f(pos, rect.x() + rect.z(), rect.y() + rect.w());
	glVertexAttrib4f(col, 0.6, 0.81, 0.92, 0.5);
	glVertexAttrib2f(pos, rect.x(), rect.y() + rect.w());
	glEnd();	

	float height = value / 100.0 * (rect.w() - INSET);
	btVector3 color3 = bottomColor.lerp(topColor, value / 100.0);
	btVector4 color(color3.x(), color3.y(), color3.z(), bottomColor.w());

	glBegin(GL_QUADS);
	glVertexAttrib4fv(col, bottomColor.m_floats);
	glVertexAttrib2f(pos, rect.x() + INSET, rect.y() + INSET);
	glVertexAttrib4fv(col, bottomColor.m_floats);
	glVertexAttrib2f(pos, rect.x() + rect.z() - INSET, rect.y() + INSET);
	glVertexAttrib4fv(col, color.m_floats);
	glVertexAttrib2f(pos, rect.x() + rect.z() - INSET, rect.y() + height);
	glVertexAttrib4fv(col, color.m_floats);
	glVertexAttrib2f(pos, rect.x() + INSET, rect.y() + height);
	glEnd();
	
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

void StatusBar::setTopColor(btVector4 color) {
	topColor = color;
}

void StatusBar::setBottomColor(btVector4 color) {
	bottomColor = color;
}

void StatusBar::print(){
	printf("%f\n", value);
}

float StatusBar::getValue() {
	return value;
}

void StatusBar::setValue(float val) {
	value = val;
}

void StatusBar::add(float val) {
	value += val;
	if(value > 100) value = 100.0;
}

void StatusBar::subtract(float val) {
	value -= val;
	if(value < 0 ) value = 0.0;
}
