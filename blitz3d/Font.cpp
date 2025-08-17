#include "Font.h"

#include "../gxruntime/gxutf8.h"

Font::Font(std::string font, int size, FT_Library ft)
{
    FT_New_Face(ft, font.c_str(), 0, &face);
    fontsize = size;
}

void Font::Set()
{
    FT_Set_Pixel_Sizes(face, 0, fontsize);
}

Font::~Font()
{
    FT_Done_Face(face);

    glDeleteVertexArrays(1, &textVAO);
    glDeleteBuffers(1, &textVBO);

    for (auto& ch : Characters) {
        glDeleteTextures(1, &ch.second.TextureID);
    }
}

void Font::Text(std::string text)
{
    codepoints.clear();
    size_t textLen = strlen(text.c_str());

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned int i = 0; i < textLen;)
    {
        unsigned int charLen = UTF8::measureCodepoint(text[i]);
        unsigned int unicode = UTF8::decodeCharacter(text.c_str(), i);

        unsigned int c = unicode;
        FT_Load_Char(face, c, FT_LOAD_RENDER);

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            MD_Math::IVECTOR2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            MD_Math::IVECTOR2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<unsigned int, Character>(c, character));

        codepoints.push_back(unicode);
        i += charLen;
    }

    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 5, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Font::Draw(float x, float y, float z, float scale)
{
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (unsigned int c : codepoints) {

        auto it = Characters.find(c);
        it == Characters.end();

        Character ch = it->second;

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float vertices[6][5] = {
            { xpos,     ypos + h, z,  0.0f, 0.0f },
            { xpos,     ypos,     z,  0.0f, 1.0f },
            { xpos + w, ypos,     z,  1.0f, 1.0f },
            { xpos,     ypos + h, z,  0.0f, 0.0f },
            { xpos + w, ypos,     z,  1.0f, 1.0f },
            { xpos + w, ypos + h, z,  1.0f, 0.0f }
        };
        
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.Advance >> 6) * scale;

    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
    
}