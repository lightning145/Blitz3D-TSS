#ifndef IBL_H
#define IBL_H

#include "Shader.h"
#include "Mesh.h"

#include <glad/glad.h>

class IBL
{
private:
	unsigned int FBO, RBO;
	unsigned int texture_fbo;
    unsigned int quadVAO, quadVBO;

public:
	unsigned int envCubemap;
	unsigned int irradianceMap;
	unsigned int prefilterMap;
	unsigned int brdfLUTTexture;

	Cube* FinalSkyBox;

	IBL() {};
	IBL(const char* hdrfile);
	~IBL() {
		glDeleteFramebuffers(1, &FBO);
		glDeleteRenderbuffers(1, &RBO);
		glDeleteTextures(1, &envCubemap);
		glDeleteTextures(1, &irradianceMap);
		glDeleteTextures(1, &prefilterMap);
		glDeleteTextures(1, &brdfLUTTexture);
		delete FinalSkyBox;
	};

};

#endif