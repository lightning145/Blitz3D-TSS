#ifndef SHADER_H
#define SHADER_H

#include "MD_Math.h"

class Shader
{
private:
    const char* VSS, *FSS;
    const char* ReadFile(const char* f);

public:
    unsigned int Program;

    Shader(){};
    Shader(const char* VSs, const char* FSs);
    ~Shader();

    void Link();
    void Use();

    void SetInt(const char* name, int value);
    void SetFloat(const char* name, float value);

    void SetVec2(const char* name, MD_Math::VECTOR2 value);
    void SetVec3(const char* name, MD_Math::VECTOR3 value);
    void SetVec4(const char* name, MD_Math::VECTOR4 value);

    void SetMatrix(const char* name, MD_Math::MATRIX value);

};



#endif