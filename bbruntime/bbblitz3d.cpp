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

MATRIX projection = IdentityMatrix();

//-------------------Camera-------------------------
void CreateCamera()
{
    camera = new Camera();
    camera->SetPos(VECTOR3(0.0f, 0.0f, 1.0f));
    cameraRun = true;
    //return camera;
}

void SetCameraRange(float c_near, float c_far)
{
    projection = PerspectiveMatrixRH(
        AngularToRadian(45.0f), (float)gx_runtime->GetWidth() / (float)gx_runtime->GetHeight() ,
        c_near, c_far);
}

void SetCamera(float foot_speed, float mouse_speed)
{
    camera->Move(foot_speed, mouse_speed);
}

void FreeCamera()
{
    delete camera;
}

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

void ApplyNormalMapForCube(Cube* model, int normal)
{
    model->shader.Use();
    model->shader.SetInt("UseNormalMap", true);
    model->shader.SetInt("texture_normal0", 1);
    SetTexture(normal, GL_TEXTURE0 + 1);
}

void ApplyNormalMapForQuad(Quad* model, int normal)
{
    model->shader.Use();
    model->shader.SetInt("UseNormalMap", true);
    model->shader.SetInt("texture_normal0", 1);
    SetTexture(normal, GL_TEXTURE0 + 1);
}

//-----------------------Light--------------------------------------

PBR_Light* CreateLight()
{
    PBR_Light* p_light = new PBR_Light(VECTOR3(0.0f, 0.0f, 0.0f),
        VECTOR3(1.0f,1.0f, 1.0f),
        VECTOR3(0.0f, 0.0f, 0.0f));

    lightRun = true;

    return p_light;
}

void SetModelAttrib(Model* model, float metaliic, float roughness)
{
    model->modelShader.Use();
    model->modelShader.SetFloat("mmm", metaliic);
    model->modelShader.SetFloat("rrr", roughness);
}

void SetCubeAttrib(Cube* model, float metaliic, float roughness)
{
    model->shader.Use();
    model->shader.SetFloat("mmm", metaliic);
    model->shader.SetFloat("rrr", roughness);
}

void SetQuadAttrib(Quad* model, float metaliic, float roughness)
{
    model->shader.Use();
    model->shader.SetFloat("mmm", metaliic);
    model->shader.SetFloat("rrr", roughness);
}

void SetLightDirectional(PBR_Light* light, float x, float y, float z)
{
    light->Directional = VECTOR3(x, y, z);
}

void SetLightPos(PBR_Light* light, float x, float y, float z)
{
    light->Position = VECTOR3(x, y, z);
}

void SetLightColor(PBR_Light* light, float r, float g, float b)
{
    light->Color = VECTOR3(r ,g ,b);
}

void EnableDirectionLight(PBR_Light* light ,Model* model)
{
    model->modelShader.Use();
    model->modelShader.SetInt("UseDirection", true);
    model->modelShader.SetVec3("LightDirectional", light->Directional);
}

void EnableDirectionLightCube(PBR_Light* light, Cube* model)
{
    model->shader.Use();
    model->shader.SetInt("UseDirection", true);
    model->shader.SetVec3("LightDirectional", light->Directional);
}

void EnableDirectionLightQuad(PBR_Light* light, Quad* model)
{
    model->shader.Use();
    model->shader.SetInt("UseDirection", true);
    model->shader.SetVec3("LightDirectional", light->Directional);
}

void ApplyLightForModel(PBR_Light* light, Model* model)
{
    model->modelShader.Use();
    if (lightRun)
        model->modelShader.SetInt("Uselight", true);
    model->modelShader.SetVec3("LightColor", light->Color);
    model->modelShader.SetVec3("LightPos", light->Position);
}

void ApplyLightForCube(PBR_Light* light, Cube* cube)
{
    cube->shader.Use();
    if (lightRun)
        cube->shader.SetInt("Uselight", true);
    cube->shader.SetVec3("LightColor", light->Color);
    cube->shader.SetVec3("LightPos", light->Position);
}

void ApplyLightForQuad(PBR_Light* light, Quad* quad)
{
    quad->shader.Use();
    if (lightRun)
        quad->shader.SetInt("Uselight", true);
    quad->shader.SetVec3("LightColor", light->Color);
    quad->shader.SetVec3("LightPos", light->Position);
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

void ShadowModel(Model* model, PBR_Light* light,float x, float y, float z,
                               float a, float b, float c, float d)
{
    PLANE plane(a, b, c, d);

    model->modelShader.Use();
    model->modelShader.SetVec3("LightColor", VECTOR3(0.0f, 0.0f, 0.0f));
    model->modelShader.SetMatrix("model", TranslationMatrix(x, y, z) * ScaleMatrix(0.001f, 0.001f, 0.001f) * ShadowMatrix(
        VECTOR4(light->Position.x, light->Position.y, light->Position.z, 1.0f),plane
    ));
}

//-------------------Model--------------------------
Model* LoadModel(BBStr* modelpath)
{
    Shader shader = Shader(model_vs, model_fs);
    Model* model = new Model(modelpath->c_str(), shader);
    model->modelShader.Use();
    model->modelShader.SetMatrix("projection", projection);
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
    Shader shader = Shader(Cube_vs, Cube_fs);
    Cube* cube = new Cube(shader);
    cube->shader.Use();
    cube->shader.SetMatrix("projection", projection);
    cube->shader.SetMatrix("view", ViewMatrixRH(VECTOR3(0.0f, 0.0f, 1.0f), VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f)));
    cube->shader.SetMatrix("model", TranslationMatrix(0.0f, 0.0f, 0.0f));
    return cube;
}

void DrawCube(Cube* cube)
{
    cube->shader.Use();
    if(cameraRun)
        cube->shader.SetMatrix("view", camera->Matrix()); 
        cube->shader.SetVec3("ViewPos", camera->Pos());
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
    ibl->FinalSkyBox->shader.SetMatrix("projection", projection);
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

void ApplySkyBoxIBLForQuad(IBL* ibl, Quad* model)
{
    model->shader.Use();
    model->shader.SetInt("irradianceMap", 2);
    model->shader.SetInt("prefilterMap", 3);
    model->shader.SetInt("brdfLUT", 4);
    model->shader.SetInt("UseIBL", true);

    SetCubeTexture(ibl->irradianceMap, GL_TEXTURE0 + 2);
    SetCubeTexture(ibl->prefilterMap, GL_TEXTURE0 + 3);
    SetTexture(ibl->brdfLUTTexture, GL_TEXTURE0 + 4);
}

void ApplySkyBoxIBLForCube(IBL* ibl, Cube* model)
{
    model->shader.Use();
    model->shader.SetInt("irradianceMap", 2);
    model->shader.SetInt("prefilterMap", 3);
    model->shader.SetInt("brdfLUT", 4);
    model->shader.SetInt("UseIBL", true);

    SetCubeTexture(ibl->irradianceMap, GL_TEXTURE0 + 2);
    SetCubeTexture(ibl->prefilterMap, GL_TEXTURE0 + 3);
    SetTexture(ibl->brdfLUTTexture, GL_TEXTURE0 + 4);
}

//----------------------Quad---------------------
Quad* CreateQuad()
{
    Shader shader = Shader(Cube_vs, Cube_fs);
    Quad* quad = new Quad(shader);
    quad->shader.Use();
    quad->shader.SetMatrix("projection", projection);
    quad->shader.SetMatrix("view", ViewMatrixRH(VECTOR3(0.0f, 0.0f, 1.0f), VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f)));
    quad->shader.SetMatrix("model", TranslationMatrix(0.0f, 0.0f, 0.0f));
    return quad;
}

void DrawQuad(Quad* quad)
{
    quad->shader.Use();
    if (cameraRun)
        quad->shader.SetMatrix("view", camera->Matrix()); 
        quad->shader.SetVec3("ViewPos", camera->Pos());

    quad->Draw();
}

void FreeQuad(Quad* quad)
{
    delete quad;
}

void SetQuad(Quad* quad ,float x, float y, float z, 
                         float sx, float sy, float sz,
                         float rx, float ry, float rz)
{
    quad->shader.Use();
    quad->shader.SetMatrix("model",
        TranslationMatrix(x, y, z) *
        RotationMatrix(AngularToRadian(rx), 'X') * RotationMatrix(AngularToRadian(ry), 'Y') * RotationMatrix(AngularToRadian(rz), 'Z')
        * ScaleMatrix(sx, sy, sz)
        );
}

//-------------------Shadow--------------------



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
    rtSym("ShadowModel%m%l#x#y#z#a#b#c#d", ShadowModel);
    rtSym("%CreateSkyBoxIBL$hdr", CreateSkyBoxIBL);
    rtSym("DrawSkyBoxIBL%ibl", DrawSkyBoxIBL);
    rtSym("FreeSkyBoxIBL%ibl", FreeSkyBoxIBL);
    rtSym("ApplySkyBoxIBLForModel%ibl%model", ApplySkyBoxIBLForModel);
    rtSym("ApplySkyBoxIBLForCube%ibl%model", ApplySkyBoxIBLForCube);
    rtSym("ApplySkyBoxIBLForQuad%ibl%model", ApplySkyBoxIBLForQuad);

    rtSym("%LoadNormalMap$file", LoadNormalMap);
    rtSym("ApplyNormalMapForModel%model%tex", ApplyNormalMapForModel);
    rtSym("ApplyNormalMapForCube%model%tex", ApplyNormalMapForCube);
    rtSym("ApplyNormalMapForQuad%model%tex", ApplyNormalMapForQuad);

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
    rtSym("SetCameraRange#n#f", SetCameraRange);

    rtSym("%CreateCube", CreateCube);
    rtSym("DrawCube%cube", DrawCube);
    rtSym("FreeCube%cube", FreeCube);

    rtSym("%CreateQuad", CreateQuad);
    rtSym("DrawQuad%quad", DrawQuad);
    rtSym("FreeQuad%quad", FreeQuad);
    rtSym("SetQuad%quad#x#y#z#sx#sy#sz#rx#ry#rz", SetQuad);

    rtSym("%CreateLight", CreateLight);
    rtSym("SetLightPos%light#x#y#z", SetLightPos);
    rtSym("SetLightColor%light#r#g#b", SetLightColor);
    rtSym("SetLightDirectional%light#x#y#z", SetLightDirectional);
    rtSym("EnableDirectionLight%pbr%model", EnableDirectionLight);
    rtSym("EnableDirectionLightCube%pbr%model", EnableDirectionLightCube);
    rtSym("EnableDirectionLightQuad%pbr%model", EnableDirectionLightQuad);

    rtSym("ApplyLightForModel%light%model", ApplyLightForModel);
    rtSym("ApplyLightForCube%light%model", ApplyLightForCube);
    rtSym("ApplyLightForQuad%light%model", ApplyLightForQuad);

    rtSym("SetModelAttrib%model#m#r", SetModelAttrib);
    rtSym("SetCubeAttrib%cube#m#r", SetCubeAttrib);
    rtSym("SetQuadAttrib%cube#m#r", SetQuadAttrib);
}