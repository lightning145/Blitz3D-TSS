#ifndef SKYBOX_H
#define SKYBOX_H

#include "Shader.h"

class SkyBox
{
private:
    unsigned int VAO, VBO, texture;
public:
    SkyBox(){};
    SkyBox(char faces[6][128]);
    ~SkyBox();

    void Draw(Shader& shader);
};


#endif