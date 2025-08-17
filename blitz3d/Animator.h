#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Model.h"

class Animator
{
private:
    float animationTime = 0.0f;
    MD_Math::MATRIX globalInverseTransform;

    std::map<std::string, unsigned int> boneMapping;
    std::vector<BoneInfo> boneInfoList;
    unsigned int numBones = 0;

    const aiScene *scene;
    
    void ReadNodeHierarchy(float animationTime, const aiNode* node, const MD_Math::MATRIX& parentTransform);
    const aiNodeAnim* FindNodeAnim(const aiAnimation* animation, const std::string& nodeName);

    aiVector3D CalcInterpolatedPosition(float animationTime, const aiNodeAnim* nodeAnim);
    aiQuaternion CalcInterpolatedRotation(float animationTime, const aiNodeAnim* nodeAnim);
    aiVector3D CalcInterpolatedScaling(float animationTime, const aiNodeAnim* nodeAnim);

    unsigned int FindPosition(float animationTime, const aiNodeAnim* nodeAnim);
    unsigned int FindRotation(float animationTime, const aiNodeAnim* nodeAnim);
    unsigned int FindScaling(float animationTime, const aiNodeAnim* nodeAnim);
    
    float speed = 1.0f;
    int modes;

    float startFrame = 0.0f;
    float endFrame = 0.0f;    
    bool customRange = false;   

public:
    Shader modelShader;

    Animator(){};
    Animator(Model* model)
    {
        this->modelShader = model->modelShader;

        this->boneMapping = model->boneMapping;
        this->boneInfoList = model->boneInfoList;
        this->numBones = model->numBones;
        this->scene = model->scene;

    };
    ~Animator(){};
    
    void SetAnimation(int mode, float Speed);
    void UpdateAnimation(float time);
    void SetShader(Shader& shader);

    void SetAnimationRange(float start, float end) {
        startFrame = start;
        endFrame = end;
        customRange = true;
    }
};

#endif