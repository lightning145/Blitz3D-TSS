#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef __cplusplus
extern "C" {
#endif

unsigned int TextureFromFile(const char* filename);
void SetTexture(unsigned int id ,unsigned int level);
void SetCubeTexture(unsigned int id ,unsigned int level);
void FreeTexture(unsigned int id);

unsigned int LoadCubeTexture(char faces[6][128]);

#ifdef __cplusplus
}
#endif

#endif