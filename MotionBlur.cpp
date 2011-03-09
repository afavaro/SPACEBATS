/*
 *  MotionBlur.cpp
 *  CS248-Final-Project
 *
 *  Created by Jeremy Keeshin on 3/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MotionBlur.h"


MotionBlur::MotionBlur(int numFrames, int width, int height){
	buffers = new Framebuffer*[numFrames];
	this->numFrames = numFrames;
	for(int i = 0; i < numFrames; i++){
		buffers[i] = new Framebuffer(width, height);
	}
	counter = 0;
	cur = 0;
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