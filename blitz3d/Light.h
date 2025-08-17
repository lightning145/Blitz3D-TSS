#ifndef LIGHT_H
#define LIGHT_H

#include "MD_Math.h"

typedef struct Light_Point
{
    MD_Math::VECTOR3 Position;

    MD_Math::VECTOR3 Ambient;
    MD_Math::VECTOR3 Diffuse;
    MD_Math::VECTOR3 Specular;

    float Constant;
    float Linear;
    float Quadratic;
} Light_Point;

typedef struct Light_Directional
{
    MD_Math::VECTOR3 Direction;

    MD_Math::VECTOR3 Ambient;
    MD_Math::VECTOR3 Diffuse;
    MD_Math::VECTOR3 Specular;
} Light_Directional;

typedef struct Light_Spot
{
    MD_Math::VECTOR3 Position;
    MD_Math::VECTOR3 Direction;
    float cutOff;
    float outerCutOff;

    MD_Math::VECTOR3 Ambient;
    MD_Math::VECTOR3 Diffuse;
    MD_Math::VECTOR3 Specular;

    float Constant;
    float Linear;
    float Quadratic;
} Light_Spot;

typedef struct PBR_Light
{
    MD_Math::VECTOR3 Position;
    MD_Math::VECTOR3 Color;
} PBR_Light;

#endif