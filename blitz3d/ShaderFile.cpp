#include "ShaderFile.h"

const char* text_vs = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoords;\n"
"out vec2 TexCoords;\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"uniform mat4 model;\n"
"void main() {\n"
"gl_Position = projection * view * model *vec4(aPos, 1.0);\n"
"TexCoords = aTexCoords;\n"
"}\n";

const char* text_fs = 
"#version 330 core\n"
"in vec2 TexCoords;\n"
"out vec4 color;\n"
"uniform sampler2D text;\n"
"uniform vec3 textColor;\n"
"void main() {\n"
"float alpha = texture(text, TexCoords).r;\n"
"if (alpha < 0.1) discard;\n"
"vec4 sampled = vec4(1.0, 1.0, 1.0, alpha);\n"
"color = vec4(textColor, 1.0) * sampled;\n"
"}\n";

Shader text1Shader = Shader(text_vs, text_fs);

const char* fb_vs = 
"#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"layout (location = 1) in vec2 aTexCoords;\n"
"out vec2 TexCoords;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); \n"
"TexCoords = aTexCoords;\n"
"}\n";

const char* fb_fs = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoords;\n"
"uniform sampler2D screenTexture;\n"
"void main()\n"
"{\n"
"FragColor = texture(screenTexture, TexCoords);\n"
"}\n";

const char* model_vs =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNormal;\n"
"layout (location = 2) in vec2 aTexCoords;\n"
"layout (location = 3) in vec3 aTangent;\n"
"layout (location = 4) in vec3 aBitangent;\n"
"layout (location = 5) in ivec4 aBoneIDs;\n"
"layout (location = 6) in vec4 aWeights;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"uniform bool useBones = false;\n"
"out vec2 TexCoords;\n"
"const int MAX_BONES = 100;\n"
"uniform mat4 boneTransforms[MAX_BONES];\n"
"void main()\n"
"{\n"
"  mat4 boneTransform = mat4(1.0);\n"
"  if(useBones) {\n"
"      boneTransform = mat4(0.0);\n"
"      bool validBoneFound = false;\n"
"      for(int i = 0; i < 4; i++) {\n"
"         if(aBoneIDs[i] < 0) continue;\n"
"         if(aBoneIDs[i] >= MAX_BONES) {\n"
"            boneTransform = mat4(1.0);\n"
"            validBoneFound = true;\n"
"            break;\n"
"         }\n"
"         boneTransform += boneTransforms[aBoneIDs[i]] * aWeights[i];\n"
"         validBoneFound = true;\n"
"      }\n"
"      if(!validBoneFound) {\n"
"         boneTransform = mat4(1.0);\n"
"      }\n"
"  }\n"
"  vec4 pos = boneTransform * vec4(aPos, 1.0);\n"
"  gl_Position = projection * view * model * pos;\n"
"  TexCoords = aTexCoords;\n"
"}\n";

const char* model_fs = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoords;\n"
"uniform sampler2D texture_diffuse0;\n"
"void main()\n"
"{\n"
"FragColor = texture(texture_diffuse0, TexCoords);\n"
"}\n";