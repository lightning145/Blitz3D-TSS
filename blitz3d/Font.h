#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H 

#include <glad/glad.h>
#include "Shader.h"

#include <iostream>
#include <map>
#include <vector>

struct Character {
    unsigned int TextureID;
    MD_Math::IVECTOR2   Size; 
    MD_Math::IVECTOR2   Bearing;   
    unsigned int Advance;  
};

class Font
{
public:
    Font(){};
    Font(std::string font, int size, FT_Library ft);
    ~Font();

    void Set();
    void Text(std::string text);
    void Draw(float x, float y, float z, float scale);
private:
    FT_Face face;
    int fontsize;

    std::map<unsigned int, Character> Characters; 
    unsigned int textVAO, textVBO;

    std::vector<unsigned int> codepoints;
};

#endif