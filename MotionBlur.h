/*
 *  MotionBlur.h
 *  CS248-Final-Project
 *
 *  Created by Jeremy Keeshin on 3/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Framebuffer.h"
#include "Shader.h"

class MotionBlur{
	
public:	
	MotionBlur(int numFrames, int width, int height);
	~MotionBlur();
	
	Framebuffer* getFrame(int frame);
	void render(Shader* shader);
	bool shouldRenderFrame();
	
	void bind();
	void unbind();
	void update();
	
private:
	Framebuffer **buffers;
	int cur;
	int counter;
	int numFrames;
	int width, height;

	int curIndex;
};