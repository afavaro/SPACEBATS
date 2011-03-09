/*
 *  MotionBlur.h
 *  CS248-Final-Project
 *
 *  Created by Jeremy Keeshin on 3/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Framebuffer.h"


class MotionBlur{
	
public:	
	MotionBlur(int numFrames, int width, int height);
	~MotionBlur();
	
	Framebuffer* getFrame(int frame);
	
	
	void bind();
	void unbind();
	
private:
	Framebuffer **buffers;
	int cur;
	int numFrames;

};