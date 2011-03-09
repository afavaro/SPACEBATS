#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Framework.h"

class Framebuffer {
	public:
		Framebuffer(unsigned width, unsigned height);
		~Framebuffer();

		void bind();
		void unbind();

		GLuint colorTextureId();
		GLuint depthTextureId();

		GLuint getWidth();
		GLuint getHeight();

	private:
		GLuint width, height;
		GLuint frameBufferId;
		GLuint colorTexId, depthTexId;
};

#endif
