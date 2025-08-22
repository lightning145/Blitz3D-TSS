#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <map>
#include <vector>

#include "ShaderFile.h"

struct BoneInfo {
    MD_Math::MATRIX Offset;
    MD_Math::MATRIX FinalTransformation;
};

class Model
{
private:
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    void LoadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    
    Assimp::Importer import;
    aiMesh *mesh;
    aiFace face;
    aiMaterial* material;

    std::string directory;
public:
    std::map<std::string, unsigned int> boneMapping;
    std::vector<BoneInfo> boneInfoList;
    unsigned int numBones = 0;

    const aiScene *scene;

    Shader modelShader;

    Model(){};
    Model(std::string path, const char* vs, const char* fs)
    {
        modelShader = Shader(vs, fs);
        modelShader.Link();
        LoadModel(path);
    }
    
    void Draw()
    {

        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(modelShader);
    }

    void Free()
    {
        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Free();

        import.FreeScene();    

    }

    unsigned int texCount;
};

#endif