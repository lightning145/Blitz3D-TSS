#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Shader.h"

namespace FrameBuffer
{
	extern unsigned int FBO, texture_fbo, RBO;
	extern unsigned int quadVAO, quadVBO;
	extern Shader FBshader;

	void InitBuffer(int window_width, int window_height);
	void FreeBuffer();
};

#endif // !FRAMEBUFFER_H
