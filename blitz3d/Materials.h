#ifndef MATERIALS
#define MATERIALS

#include "MD_Math.h"

typedef struct Materials
{
    MD_Math::VECTOR3 Diffuse;
    MD_Math::VECTOR3 Ambient;
    MD_Math::VECTOR3 Specular;

    float Power;
} Materials;

typedef struct Materials_Texture
{
    float Power;
} Materials_Texture;


#endif