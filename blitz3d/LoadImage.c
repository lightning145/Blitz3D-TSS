
#include "LoadImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef __cplusplus
extern "C" {
#endif

void Load_Image_RGB(const char* filename, Image* image)
{
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, STBI_rgb);
    image->width = width;
    image->height = height;
    image->pixels = data;
}

void Load_Image_RGBA(const char* filename, Image* image)
{
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
    image->width = width;
    image->height = height;
    image->pixels = data;
}

#ifdef __cplusplus
}
#endif
