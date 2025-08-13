#include "Renderer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Renderer {

	void Clear(int r, int g, int b)
	{
		glClearColor((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Test()
	{
		Assimp::Importer importer;
		importer.ReadFile("", aiProcess_FlipUVs);
	}
};