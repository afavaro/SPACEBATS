/*
 *  MotionBlur.cpp
 *  CS248-Final-Project
 *
 *  Created by Jeremy Keeshin on 3/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MotionBlur.h"

#include <cstdio>
#include <vector>
using namespace std;


MotionBlur::MotionBlur(int numFrames, int width, int height){
	buffers = new Framebuffer*[numFrames];
	this->numFrames = numFrames;
	for(int i = 0; i < numFrames; i++){
		buffers[i] = new Framebuffer(width, height);
	}
	counter = 0;
	cur = 0;
	this->width = width;
	this->height = height;
}


MotionBlur::~MotionBlur(){
	for(int i = 0; i < numFrames; i++){
		delete buffers[i];
	}
	delete buffers;
}

Framebuffer* MotionBlur::getFrame(int frame){
	return buffers[frame];
}

void MotionBlur::bind(){
	getFrame(cur)->bind();
}

void MotionBlur::unbind(){
	getFrame(cur)->unbind();

	counter++;
	cur = counter % numFrames;
}


void MotionBlur::render(Shader* shader){
	glUseProgram(shader->programID());
	
	GLint frame1 = glGetUniformLocation(shader->programID(), "frame1");
	glUniform1i(frame1, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, getFrame(0)->colorTextureId());
	
	GLint frame2 = glGetUniformLocation(shader->programID(), "frame2");
	glUniform1i(frame2, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, getFrame(1)->colorTextureId());
	
	GLint frame3 = glGetUniformLocation(shader->programID(), "frame3");
	glUniform1i(frame3, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, getFrame(2)->colorTextureId());
	
	GLint frame4 = glGetUniformLocation(shader->programID(), "frame4");
	glUniform1i(frame4, 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, getFrame(3)->colorTextureId());
	
		
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
