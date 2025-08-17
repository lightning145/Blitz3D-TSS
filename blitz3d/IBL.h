#ifndef IBL_H
#define IBL_H

#include "Shader.h"
#include "Mesh.h"

class IBL
{
private:
    unsigned int FBO, RBO;
    unsigned int HDRtexture, CubemapTexture;
public:
    IBL(){};
    IBL(const char* hdr);
    ~IBL();
    
    void Set(Shader& FinalSkyBoxShader);
    void Draw(Cube& FinalSkyBox, Shader& FinalSkyBoxShader);

    unsigned int irradianceMap;
    unsigned int prefilterMap;
    unsigned int brdfLUTTexture;
};


#endif