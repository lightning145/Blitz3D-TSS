#include "ShaderFile.h"

const char* text_vs = 
"#version 460 core\n"
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
"#version 460 core\n"
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
"#version 460 core\n"
"layout (location = 0) in vec2 aPos;\n"
"layout (location = 1) in vec2 aTexCoords;\n"
"out vec2 TexCoords;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); \n"
"TexCoords = aTexCoords;\n"
"}\n";

const char* fb_fs = 
"#version 460 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoords;\n"
"uniform sampler2D screenTexture;\n"
"void main()\n"
"{\n"
"FragColor = texture(screenTexture, TexCoords);\n"
"}\n";

const char* model_vs =
"#version 460 core\n"
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
"out vec3 Normal;\n"
"out vec3 FragPos;\n"
"out mat3 TBN;\n"

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

"  FragPos = vec3(model * pos);\n"

"  mat3 nm = mat3(transpose(inverse(model)));\n"
"  mat3 boneRotation = mat3(boneTransform);\n"

"  vec3 skinnedNormal = normalize(boneRotation * aNormal);\n"
"  vec3 skinnedTangent = normalize(boneRotation * aTangent);\n"
"  vec3 skinnedBitangent = normalize(boneRotation * aBitangent);\n"

"  Normal = nm * skinnedNormal;\n"
"  vec3 T = normalize( nm * skinnedTangent);\n"
"  vec3 N = normalize( nm * skinnedNormal );\n"
"  vec3 B = normalize( nm * skinnedBitangent);\n"

"  TBN = mat3(T, B, N);\n"
"}\n";

const char* model_fs = 
"#version 460 core\n"
"out vec4 FragColor;\n"
"const float PI = 3.14159265359;\n"

"in vec2 TexCoords;\n"
"in vec3 Normal;\n"
"in vec3 FragPos;\n"
"in mat3 TBN;\n"

"uniform sampler2D texture_diffuse0;\n"
"uniform sampler2D texture_normal0;\n"

"uniform vec3 LightPos;\n"
"uniform vec3 LightColor;\n"
"uniform vec3 ViewPos;\n"

"uniform bool Uselight = false;\n"
"uniform bool UseNormalMap = false;\n"

"uniform float mmm;"
"uniform float rrr;"

"vec3 ModelColor = texture(texture_diffuse0, TexCoords).rgb;\n"

"float DistributionGGX(vec3 N, vec3 H, float roughness);\n"
"float GeometrySchlickGGX(float NdotV, float roughness);\n"
"float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);\n"
"vec3 fresnelSchlick(float cosTheta, vec3 F0);\n"
"vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);\n"

"void main()\n"
"{\n"
"    if (Uselight) {\n"
"       vec3 norm = vec3(1.0);\n"
"       vec3 LightDir = normalize(LightPos - FragPos);\n"
"       vec3 ViewDir = normalize(ViewPos - FragPos);\n"
"       vec3 HalfWayDir = normalize(LightDir + ViewDir);\n"

"       if(UseNormalMap){\n"
"          norm = texture(texture_normal0, TexCoords).xyz * 2.0 - 1.0;\n"
"          norm = normalize(TBN * norm);\n"
"       }\n"
"       else {\n"
"          norm = normalize(Normal);\n"
"       }\n"

"       vec3 albedo = pow(ModelColor , vec3(2.2));\n"
"        float metallic = mmm;\n"
"        float roughness = rrr;\n"
"        float ao = 0.0f;\n"

"        vec3 R = reflect(-ViewDir, norm);\n"

"        vec3 F0 = vec3(0.04);\n"
"        F0 = mix(F0, albedo, metallic);\n"

"        vec3 Lo = vec3(0.0);//Light Output\n"

"                float distance = length(LightPos - FragPos);\n"
"                float attenuation = 1.0 / (distance * distance);\n"
"                vec3 radiance = LightColor * attenuation;\n"

"                // Cook-Torrance BRDF\n"
"                float NDF = DistributionGGX(norm, HalfWayDir, roughness);\n"
"                float G = GeometrySmith(norm, ViewDir, LightDir, roughness);\n"
"                vec3 F = fresnelSchlick(clamp(dot(HalfWayDir, ViewDir), 0.0, 1.0), F0);\n"

"                vec3 numerator = NDF * G * F;\n"
"                float denominator = 4.0 * max(dot(norm, ViewDir), 0.0) * max(dot(norm, LightDir), 0.0) + 0.0001;\n"
"                vec3 specular = numerator / denominator;\n"

"                vec3 kS = F;\n"
"                vec3 kD = vec3(1.0) - kS;\n"
"                kD *= 1.0 - metallic;\n"

"                float NdotL = max(dot(norm, LightDir), 0.0);\n"

"                Lo += (kD * albedo / PI + specular) * radiance * NdotL;\n"

"       vec3 ambient = vec3(0.03) * albedo * ao;\n"

"       vec3 result = ambient + Lo;\n"

"       vec3 mapped = result / (result + vec3(1.0));// HDR\n"
"       mapped = pow(mapped, vec3(1.0 / 2.2));// Gamma\n"
"       FragColor = vec4(mapped, 1.0);\n"
"    }\n"
"    else {\n"
"       FragColor = vec4(ModelColor, 1.0); \n"
"    }\n"
"}\n"

"//Trowbridge-Reitz GGX\n"
"float DistributionGGX(vec3 N, vec3 H, float roughness)\n"
"{\n"
"    float a = roughness * roughness;\n"
"    float a2 = a * a;\n"
"    float NdotH = max(dot(N, H), 0.0);\n"
"    float NdotH2 = NdotH * NdotH;\n"

"    float nom = a2;\n"
"    float denom = (NdotH2 * (a2 - 1.0) + 1.0);\n"
"    denom = PI * denom * denom;\n"

"    return nom / denom;\n"
"}\n"

"//Smith¡¯s Schlick-GGX-----------------------------------------\n"
"float GeometrySchlickGGX(float NdotV, float roughness)\n"
"{\n"
"    float r = (roughness + 1.0);\n"
"    float k = (r*r) / 8.0;\n"
"    //float k = (r * r) / 2.0;\n"

"    float nom = NdotV;\n"
"    float denom = NdotV * (1.0 - k) + k;\n"

"    return nom / denom;\n"
"}\n"

"float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)\n"
"{\n"
"    float NdotV = max(dot(N, V), 0.0);\n"
"    float NdotL = max(dot(N, L), 0.0);\n"
"    float ggx2 = GeometrySchlickGGX(NdotV, roughness);\n"
"    float ggx1 = GeometrySchlickGGX(NdotL, roughness);\n"

"    return ggx1 * ggx2;\n"
"}\n"

"//Fresnel-Schlick\n"
"vec3 fresnelSchlick(float cosTheta, vec3 F0)\n"
"{\n"
"    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);\n"
"}\n"

"vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)\n"
"{\n"
"    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);\n"
"}\n"
;

const char* Cube_vs = 
"#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
"}\n";

const char* Cube_fs = 
"#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f);\n"
"}\n";