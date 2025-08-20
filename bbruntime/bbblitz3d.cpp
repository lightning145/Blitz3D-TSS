#include "std.h"

#include "bbblitz3d.h"
#include "bbgraphics.h"

#include "../blitz3d/Animator.h"
#include "../blitz3d/Camera.h"
#include "../blitz3d/Mesh.h"
#include "../blitz3d/Light.h"
#include "../blitz3d/Texture.h"

#include <glad/glad.h>

#include "../blitz3d/IBL.h"

using namespace MD_Math;

Camera* camera;

bool cameraRun = false;
bool lightRun = false;
bool normalMapRun = false;
bool IBLRun = false;

//-----------------Normal Map--------------------
int LoadNormalMap(BBStr* path)
{
    unsigned int normalMap = TextureFromFile(path->c_str());
    delete path;
    return normalMap;
}

void ApplyNormalMapForModel(Model* model, int normal)
{
    model->modelShader.Use();
    model->modelShader.SetInt("UseNormalMap", true);
    model->modelShader.SetInt("texture_normal0", model->texCount + 1);
    SetTexture(normal, GL_TEXTURE0 + model->texCount + 1);
}

PBR_Light* CreateLight()
{
    PBR_Light* p_light = new PBR_Light(VECTOR3(0.0f, 0.0f, 0.0f),
        VECTOR3(1.0f,1.0f, 1.0f));

    lightRun = true;

    return p_light;
}

void SetModelAttrib(Model* model, float metaliic, float roughness)
{
    model->modelShader.Use();
    model->modelShader.SetFloat("mmm", metaliic);
    model->modelShader.SetFloat("rrr", roughness);
}

void SetLightPos(PBR_Light* light, float x, float y, float z)
{
    light->Position = VECTOR3(x, y, z);
}

void SetLightColor(PBR_Light* light, float r, float g, float b)
{
    light->Color = VECTOR3(r ,g ,b);
}

void ApplyLightForModel(PBR_Light* light, Model* model)
{
    model->modelShader.Use();
    model->modelShader.SetVec3("LightColor", light->Color);
    model->modelShader.SetVec3("LightPos", light->Position);
}

void FreeLight(PBR_Light* light)
{
    delete light;
}

//-------------------Entity Postion----------------
void PositionCube(Cube* cube, float x, float y, float z)
{
    cube->shader.Use();
    cube->shader.SetMatrix("model", TranslationMatrix(x, y, z) * ScaleMatrix(0.1f, 0.1f, 0.1f));
}

void PositionModel(Model* model, float x, float y, float z)
{
    model->modelShader.Use();
    model->modelShader.SetMatrix("model", TranslationMatrix(x, y, z) * ScaleMatrix(0.001f, 0.001f, 0.001f));
}

//-------------------Camera-------------------------
void CreateCamera()
{
    camera = new Camera();
    camera->SetPos(VECTOR3(0.0f, 0.0f, 1.0f));
    cameraRun = true;
    //return camera;
}

void SetCamera(float foot_speed, float mouse_speed)
{
    camera->Move(foot_speed, mouse_speed);
}

void FreeCamera()
{
    delete camera;
}

//-------------------Model--------------------------
Model* LoadModel(BBStr* modelpath)
{
    Model* model = new Model(modelpath->c_str());
    model->modelShader.Use();
    model->modelShader.SetMatrix("projection", PerspectiveMatrixRH(AngularToRadian(45.0f), (float)gx_runtime->GetWidth() / (float)gx_runtime->GetHeight(), 0.1f, 100.0f));
    model->modelShader.SetMatrix("view", ViewMatrixRH(VECTOR3(0.0f, 0.0f, 1.0f), VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f)));
    model->modelShader.SetMatrix("model", TranslationMatrix(0.0f, 0.0f, 0.0f) * ScaleMatrix(0.001f, 0.001f, 0.001f));
    delete modelpath;
    return model;
}

void DrawModel(Model* model)
{
    model->modelShader.Use();
    if(cameraRun)
        model->modelShader.SetMatrix("view", camera->Matrix());
        model->modelShader.SetVec3("ViewPos", camera->Pos());

    if (lightRun)
        model->modelShader.SetInt("Uselight", true);
    model->Draw();
}

void FreeModel(Model* model)
{
    model->Free();
    delete model;
}

//-------------------Animation-----------------------

Animator* CreateAnimation(Model* model)
{
    Animator* Anim = new Animator(model);
    return Anim;
}

void PlayAnimation(Animator* animator, float currentTime, int mode, float speed, int begin, int end)
{

    animator->SetAnimation(mode, speed);

    if (begin >= 0 && end > begin) {
        animator->SetAnimationRange(static_cast<float>(begin), static_cast<float>(end));
    }

    animator->UpdateAnimation(currentTime);

    animator->modelShader.Use();
    animator->modelShader.SetInt("useBones", (int)true);
    animator->SetShader(animator->modelShader);
}

void FreeAnimation(Animator* animator)
{
    if (animator) {
        delete animator;
    }
}

//-------------------Cube---------------------------

Cube* CreateCube()
{
    Cube* cube = new Cube(Cube_vs, Cube_fs);
    cube->shader.Link();
    cube->shader.Use();
    cube->shader.SetMatrix("projection", PerspectiveMatrixRH(AngularToRadian(45.0f), (float)gx_runtime->GetWidth() / (float)gx_runtime->GetHeight(), 0.1f, 100.0f));
    cube->shader.SetMatrix("view", ViewMatrixRH(VECTOR3(0.0f, 0.0f, 1.0f), VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f)));
    cube->shader.SetMatrix("model", TranslationMatrix(0.0f, 0.0f, 0.0f));
    return cube;
}

void DrawCube(Cube* cube)
{
    cube->shader.Use();
    if(cameraRun)
        cube->shader.SetMatrix("view", camera->Matrix());
    cube->Draw();
}

void FreeCube(Cube* cube)
{
    delete cube;
}

//---------IBL--------------------------}
IBL* CreateSkyBoxIBL(BBStr* hdr)
{
    IBL* ibl = new IBL(hdr->c_str());
    delete hdr;
    return ibl;
}

void DrawSkyBoxIBL(IBL* ibl)
{
    GLboolean depthMask;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);
    GLint depthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);

    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    ibl->FinalSkyBox->shader.Use();
    ibl->FinalSkyBox->shader.SetMatrix("view", camera->Matrix());
    ibl->FinalSkyBox->shader.SetMatrix("projection", PerspectiveMatrixRH(AngularToRadian(45.0f), (float)gx_runtime->GetWidth() / (float)gx_runtime->GetHeight(), 0.1f, 100.0f));
    ibl->FinalSkyBox->shader.SetInt("environmentMap", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ibl->envCubemap);
    ibl->FinalSkyBox->Draw();

    glDepthMask(depthMask);
    glDepthFunc(depthFunc);
}

void FreeSkyBoxIBL(IBL* ibl)
{
    delete ibl;
}

void ApplySkyBoxIBLForModel(IBL* ibl, Model* model)
{
    model->modelShader.Use();
    model->modelShader.SetInt("irradianceMap", model->texCount + 2);
    model->modelShader.SetInt("prefilterMap", model->texCount + 3);
    model->modelShader.SetInt("brdfLUT", model->texCount + 4);
    model->modelShader.SetInt("UseIBL", true);

    SetCubeTexture(ibl->irradianceMap, GL_TEXTURE0 + model->texCount + 2);
    SetCubeTexture(ibl->prefilterMap, GL_TEXTURE0 + model->texCount + 3);
    SetTexture(ibl->brdfLUTTexture, GL_TEXTURE0 + model->texCount + 4);
}

void blitz3d_open() {
	
}

void blitz3d_close() {
	
}

bool blitz3d_create() {
	
	return true;
	
}

bool blitz3d_destroy() {
	blitz3d_close();
	return true;
}

void blitz3d_link(void (*rtSym)(const char* sym, void* pc)) {
    rtSym("%CreateSkyBoxIBL$hdr", CreateSkyBoxIBL);
    rtSym("DrawSkyBoxIBL%ibl", DrawSkyBoxIBL);
    rtSym("FreeSkyBoxIBL%ibl", FreeSkyBoxIBL);
    rtSym("ApplySkyBoxIBLForModel%ibl%model", ApplySkyBoxIBLForModel);

    rtSym("%LoadNormalMap$file", LoadNormalMap);
    rtSym("ApplyNormalMapForModel%model%tex", ApplyNormalMapForModel);

    rtSym("PositionModel%model#x#y#z", PositionModel);
    rtSym("PositionCube%cube#x#y#z", PositionCube);

    rtSym("%LoadModel$model", LoadModel);
    rtSym("DrawModel%model", DrawModel);
    rtSym("FreeModel%model", FreeModel);

    rtSym("%CreateAnimation%model", CreateAnimation);
    rtSym("PlayAnimation%anim#currTime%mode#speed%begin%end", PlayAnimation);
    rtSym("FreeAnimation%anim", FreeAnimation);

    rtSym("CreateCamera", CreateCamera);
    rtSym("SetCamera#f#m", SetCamera);
    rtSym("FreeCamera", FreeCamera);

    rtSym("%CreateCube", CreateCube);
    rtSym("DrawCube%cube", DrawCube);
    rtSym("FreeCube%cube", FreeCube);

    rtSym("%CreateLight", CreateLight);
    rtSym("SetLightPos%light#x#y#z", SetLightPos);
    rtSym("SetLightColor%light#r#g#b", SetLightColor);

    rtSym("ApplyLightForModel%light%model", ApplyLightForModel);

    rtSym("SetModelAttrib%model#m#r", SetModelAttrib);
}