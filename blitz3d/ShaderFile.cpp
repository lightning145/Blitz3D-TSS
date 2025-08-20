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
"if (alpha < 0.2) discard;\n"
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
"       vec3 result = texture(screenTexture, TexCoords).rgb;\n"
"       FragColor = vec4(result ,1.0);\n"
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
"uniform samplerCube irradianceMap;\n"
"uniform samplerCube prefilterMap;\n"
"uniform sampler2D   brdfLUT;\n"

"uniform vec3 LightPos;\n"
"uniform vec3 LightColor;\n"
"uniform vec3 ViewPos;\n"

"uniform bool Uselight = false;\n"
"uniform bool UseNormalMap = false;\n"
"uniform bool UseIBL = false;\n"

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
"        float ao = 1.0f;\n"

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

"        vec3 ambient = vec3(1.0);\n"
"        vec3 irradiance = texture(irradianceMap, norm).rgb; \n"
"        vec3 diffuse = irradiance * albedo; \n"
"        const float MAX_REFLECTION_LOD = 4.0; \n"
"        vec3 prefilteredColor = textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb; \n"
"        vec2 brdf = texture(brdfLUT, vec2(max(dot(norm, ViewDir), 0.0), roughness)).rg; \n"

"       if(UseIBL)\n"
"       {\n"
"             kS = fresnelSchlickRoughness(max(dot(norm, ViewDir), 0.0), F0, roughness);\n"
"             kD = vec3(1.0) - kS; \n"
"             kD *= 1.0 - metallic; \n"
"             specular = prefilteredColor * (F * brdf.x + brdf.y);\n"
"             ambient = (kD * diffuse + specular) * ao; \n"
"       }\n"
"       else {\n"
"            ambient = vec3(0.03) * albedo * ao;\n"
"       }\n"

"       vec3 result = ambient + Lo;\n"

"       result = result / (result + vec3(1.0));// HDR\n"
"       result = pow(result, vec3(1.0 / 2.2));// Gamma\n"
"       FragColor = vec4(result, 1.0);\n"
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
"    float k = 1.0;\n"
"       if(UseIBL)\n"
"       {\n"
"           k = (r * r) / 2.0;\n"
"       }\n"
"       else {\n"
"           k = (r*r) / 8.0;\n"
"       }\n"

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

//------------------IBL----------------

const char* ToCubemap_vs = 
"#version 330 core\n"
"layout(location = 0) in vec3 aPos; \n"

"out vec3 localPos; \n"

"uniform mat4 projection; \n"
"uniform mat4 view; \n"

"void main()\n"
"{\n"
"   localPos = aPos; \n"
"    gl_Position = projection * view * vec4(localPos, 1.0); \n"
"}\n";

const char* ToCubemap_fs = 
"#version 330 core\n"
"out vec4 FragColor; \n"
"in vec3 localPos; \n"

"uniform sampler2D equirectangularMap; \n"

"const vec2 invAtan = vec2(0.1591, 0.3183); \n"
"vec2 SampleSphericalMap(vec3 v)\n"
"{\n"
"    vec2 uv = vec2(atan(v.z, v.x), asin(v.y)); \n"
"    uv *= invAtan; \n"
"    uv += 0.5; \n"
"    return uv; \n"
"}\n"

"void main()\n"
"{\n"
"    vec2 uv = SampleSphericalMap(normalize(localPos)); // make sure to normalize localPos\n"
"    vec3 color = texture(equirectangularMap, uv).rgb; \n"

"    FragColor = vec4(color, 1.0); \n"
"}\n";

const char* irradiance_fs = 
"#version 330 core\n"
"out vec4 FragColor; \n"
"in vec3 WorldPos; \n"

"uniform samplerCube environmentMap; \n"

"const float PI = 3.14159265359; \n"

"void main()\n"
"{\n"
"    // The world vector acts as the normal of a tangent surface\n"
"    // from the origin, aligned to WorldPos. Given this normal, calculate all\n"
"    // incoming radiance of the environment. The result of this radiance\n"
"    // is the radiance of light coming from -Normal direction, which is what\n"
"    // we use in the PBR shader to sample irradiance.\n"
"    vec3 N = normalize(WorldPos); \n"

"    vec3 irradiance = vec3(0.0); \n"

"    // tangent space calculation from origin point\n"
"    vec3 up = vec3(0.0, 1.0, 0.0); \n"
"    vec3 right = normalize(cross(up, N)); \n"
"    up = normalize(cross(N, right)); \n"

"    float sampleDelta = 0.025; \n"
"    float nrSamples = 0.0; \n"
"    for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)\n"
"    {\n"
"        for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)\n"
"        {\n"
"            // spherical to cartesian (in tangent space)\n"
"            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta)); \n"
"            // tangent space to world\n"
"            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N; \n"

"            irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta); \n"
"            nrSamples++; \n"
"        }\n"
"    }\n"
"    irradiance = PI * irradiance * (1.0 / float(nrSamples)); \n"

"    FragColor = vec4(irradiance, 1.0); \n"
"}\n";

const char* prefilter_fs = 
"#version 330 core\n"
"out vec4 FragColor; \n"
"in vec3 localPos; \n"

"uniform samplerCube environmentMap; \n"
"uniform float roughness; \n"

"const float PI = 3.14159265359; \n"

"float DistributionGGX(vec3 N, vec3 H, float roughness); \n"
"float RadicalInverse_VdC(uint bits); \n"
"vec2 Hammersley(uint i, uint N); \n"
"vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness); \n"

"void main()\n"
"{\n"
"    vec3 N = normalize(localPos); \n"
"    vec3 R = N; \n"
"    vec3 V = R; \n"

"    const uint SAMPLE_COUNT = 1024u; \n"
"    float totalWeight = 0.0; \n"
"    vec3 prefilteredColor = vec3(0.0); \n"
"    for (uint i = 0u; i < SAMPLE_COUNT; ++i)\n"
"    {\n"
"        vec2 Xi = Hammersley(i, SAMPLE_COUNT); \n"
"        vec3 H = ImportanceSampleGGX(Xi, N, roughness); \n"
"        vec3 L = normalize(2.0 * dot(V, H) * H - V); \n"

"        float NdotL = max(dot(N, L), 0.0); \n"
"        if (NdotL > 0.0)\n"
"        {\n"
"            prefilteredColor += texture(environmentMap, L).rgb * NdotL; \n"
"            totalWeight += NdotL; \n"
"        }\n"
"    }\n"
"    prefilteredColor = prefilteredColor / totalWeight; \n"

"    FragColor = vec4(prefilteredColor, 1.0); \n"
"}\n"

"float DistributionGGX(vec3 N, vec3 H, float roughness)\n"
"{\n"
"    float a = roughness * roughness; \n"
"    float a2 = a * a; \n"
"    float NdotH = max(dot(N, H), 0.0); \n"
"    float NdotH2 = NdotH * NdotH; \n"

"    float nom = a2; \n"
"    float denom = (NdotH2 * (a2 - 1.0) + 1.0); \n"
"    denom = PI * denom * denom; \n"

"    return nom / denom; \n"
"}\n"

"// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html\n"
"// efficient VanDerCorpus calculation.\n"
"float RadicalInverse_VdC(uint bits)\n"
"{\n"
"    bits = (bits << 16u) | (bits >> 16u); \n"
"    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u); \n"
"    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u); \n"
"    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u); \n"
"    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u); \n"
"    return float(bits) * 2.3283064365386963e-10; // / 0x100000000\n"
"}\n"
"// ----------------------------------------------------------------------------\n"
"vec2 Hammersley(uint i, uint N)\n"
"{\n"
"    return vec2(float(i) / float(N), RadicalInverse_VdC(i)); \n"
"}\n"

"vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)\n"
"{\n"
"    float a = roughness * roughness; \n"

"    float phi = 2.0 * PI * Xi.x; \n"
"    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y)); \n"
"    float sinTheta = sqrt(1.0 - cosTheta * cosTheta); \n"

"    // from spherical coordinates to cartesian coordinates\n"
"    vec3 H; \n"
"    H.x = cos(phi) * sinTheta; \n"
"    H.y = sin(phi) * sinTheta; \n"
"    H.z = cosTheta; \n"

"    // from tangent-space vector to world-space sample vector\n"
"    vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0); \n"
"    vec3 tangent = normalize(cross(up, N)); \n"
"    vec3 bitangent = cross(N, tangent); \n"

"    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z; \n"
"    return normalize(sampleVec); \n"
"}\n"; 

const char* brdf_vs = 
"#version 330 core\n"
"layout(location = 0) in vec3 aPos; \n"
"layout(location = 1) in vec2 aTexCoords; \n"

"out vec2 TexCoords; \n"

"void main()\n"
"{\n"
"	TexCoords = aTexCoords; \n"
"	gl_Position = vec4(aPos, 1.0); \n"
"}\n";

const char* brdf_fs = 
"#version 330 core\n"
"out vec2 FragColor; \n"
"in vec2 TexCoords; \n"

"const float PI = 3.14159265359; \n"
"// ----------------------------------------------------------------------------\n"
"// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html\n"
"// efficient VanDerCorpus calculation.\n"
"float RadicalInverse_VdC(uint bits)\n"
"{\n"
"    bits = (bits << 16u) | (bits >> 16u); \n"
"    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u); \n"
"    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u); \n"
"    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u); \n"
"    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u); \n"
"    return float(bits) * 2.3283064365386963e-10; // / 0x100000000\n"
"}\n"
"// ----------------------------------------------------------------------------\n"
"vec2 Hammersley(uint i, uint N)\n"
"{\n"
"    return vec2(float(i) / float(N), RadicalInverse_VdC(i)); \n"
"}\n"
"// ----------------------------------------------------------------------------\n"
"vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)\n"
"{\n"
"    float a = roughness * roughness; \n"

"    float phi = 2.0 * PI * Xi.x; \n"
"    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y)); \n"
"    float sinTheta = sqrt(1.0 - cosTheta * cosTheta); \n"

"    // from spherical coordinates to cartesian coordinates - halfway vector\n"
"    vec3 H; \n"
"    H.x = cos(phi) * sinTheta; \n"
"    H.y = sin(phi) * sinTheta; \n"
"    H.z = cosTheta; \n"

"    // from tangent-space H vector to world-space sample vector\n"
"    vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0); \n"
"    vec3 tangent = normalize(cross(up, N)); \n"
"    vec3 bitangent = cross(N, tangent); \n"

"    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z; \n"
"    return normalize(sampleVec); \n"
"}\n"
"// ----------------------------------------------------------------------------\n"
"float GeometrySchlickGGX(float NdotV, float roughness)\n"
"{\n"
"    // note that we use a different k for IBL\n"
"    float a = roughness; \n"
"    float k = (a * a) / 2.0; \n"

"    float nom = NdotV; \n"
"    float denom = NdotV * (1.0 - k) + k; \n"

"    return nom / denom; \n"
"}\n"
"// ----------------------------------------------------------------------------\n"
"float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)\n"
"{\n"
"    float NdotV = max(dot(N, V), 0.0); \n"
"    float NdotL = max(dot(N, L), 0.0); \n"
"    float ggx2 = GeometrySchlickGGX(NdotV, roughness); \n"
"    float ggx1 = GeometrySchlickGGX(NdotL, roughness); \n"

"    return ggx1 * ggx2; \n"
"}\n"
"// ----------------------------------------------------------------------------\n"
"vec2 IntegrateBRDF(float NdotV, float roughness)\n"
"{\n"
"    vec3 V; \n"
"    V.x = sqrt(1.0 - NdotV * NdotV); \n"
"    V.y = 0.0; \n"
"    V.z = NdotV; \n"

"    float A = 0.0; \n"
"    float B = 0.0; \n"

"    vec3 N = vec3(0.0, 0.0, 1.0); \n"

"    const uint SAMPLE_COUNT = 1024u; \n"
"    for (uint i = 0u; i < SAMPLE_COUNT; ++i)\n"
"    {\n"
"        // generates a sample vector that's biased towards the\n"
"        // preferred alignment direction (importance sampling).\n"
"        vec2 Xi = Hammersley(i, SAMPLE_COUNT); \n"
"        vec3 H = ImportanceSampleGGX(Xi, N, roughness); \n"
"        vec3 L = normalize(2.0 * dot(V, H) * H - V); \n"

"        float NdotL = max(L.z, 0.0); \n"
"        float NdotH = max(H.z, 0.0); \n"
"        float VdotH = max(dot(V, H), 0.0); \n"

"        if (NdotL > 0.0)\n"
"        {\n"
"            float G = GeometrySmith(N, V, L, roughness); \n"
"            float G_Vis = (G * VdotH) / (NdotH * NdotV); \n"
"            float Fc = pow(1.0 - VdotH, 5.0); \n"

"            A += (1.0 - Fc) * G_Vis; \n"
"            B += Fc * G_Vis; \n"
"        }\n"
"    }\n"
"    A /= float(SAMPLE_COUNT); \n"
"    B /= float(SAMPLE_COUNT); \n"
"    return vec2(A, B); \n"
"}\n"
"// ----------------------------------------------------------------------------\n"
"void main()\n"
"{\n"
"    vec2 integratedBRDF = IntegrateBRDF(TexCoords.x, TexCoords.y); \n"
"    FragColor = integratedBRDF; \n"
"}\n";

const char* SkyBox_vs = 
"#version 330 core\n"
"layout(location = 0) in vec3 aPos; \n"

"uniform mat4 projection; \n"
"uniform mat4 view; \n"

"out vec3 localPos; \n"

"void main()\n"
"{\n"
"    localPos = aPos; \n"

"    mat4 rotView = mat4(mat3(view)); // remove translation from the view matrix\n"
"    vec4 clipPos = projection * rotView * vec4(localPos, 1.0); \n"

"    gl_Position = clipPos.xyww; \n"
"}\n";

const char* SkyBox_fs = 
"#version 330 core\n"
"out vec4 FragColor; \n"

"in vec3 localPos; \n"

"uniform samplerCube environmentMap; \n"

"void main()\n"
"{\n"
"    vec3 envColor = textureLod(environmentMap, localPos, 1.2).rgb; \n"

"    //HDR And Gamma\n"
"    envColor = envColor / (envColor + vec3(1.0)); \n"
"    envColor = pow(envColor, vec3(1.0 / 2.2)); \n"

"    FragColor = vec4(envColor, 1.0); \n"
"}\n";
