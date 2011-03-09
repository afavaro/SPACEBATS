
#include "Framebuffer.h"

using namespace std;

Framebuffer::Framebuffer(unsigned width, unsigned height) {
	this->width = width;
	this->height = height;

	glGenTextures(1, &depthTexId);
	glBindTexture(GL_TEXTURE_2D, depthTexId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height,
			0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glGenTextures(1, &colorTexId);
	glBindTexture(GL_TEXTURE_2D, colorTexId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, width, height,
			0, GL_RGBA, GL_FLOAT, 0);

	glGenFramebuffersEXT(1, &frameBufferId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferId);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
			GL_TEXTURE_2D, depthTexId, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
			GL_TEXTURE_2D, colorTexId, 0);

	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT) {
		cerr << "Invalid framebuffer configuration" << endl;
		exit(-1);
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffersEXT(1, &frameBufferId);
	glDeleteTextures(1, &depthTexId);
	glDeleteTextures(1, &colorTexId);
}

GLuint Framebuffer::colorTextureId() {
	return colorTexId;
}

GLuint Framebuffer::depthTextureId() {
	return depthTexId;
}

GLuint Framebuffer::getWidth() {
	return width;
}

GLuint Framebuffer::getHeight() {
	return height;
}

void Framebuffer::bind() {
	glPushAttrib(GL_VIEWPORT_BIT);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferId);
	glViewport(0, 0, width, height);
}

void Framebuffer::unbind() {
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glPopAttrib();
}
