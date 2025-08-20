#ifndef SHADERFILE_H
#define SHADERFILE_H

#include "Shader.h"

extern const char* text_vs;
extern const char* text_fs;
extern Shader text1Shader;

extern const char* fb_vs;
extern const char* fb_fs;

extern const char* model_vs;
extern const char* model_fs;

extern const char* Cube_vs;
extern const char* Cube_fs;

//---------------IBL------------------

extern const char* ToCubemap_vs;
extern const char* ToCubemap_fs;

extern const char* irradiance_fs;
extern const char* prefilter_fs;

extern const char* brdf_vs;
extern const char* brdf_fs;

extern const char* SkyBox_vs;
extern const char* SkyBox_fs;

//--------------Shadow---------------

extern const char* DepthMap_vs;
extern const char* DepthMap_fs;
#endif