#include "std.h"

#include "bbblitz3d.h"
#include "bbgraphics.h"

#include "../blitz3d/Animator.h"

using namespace MD_Math;

Model* LoadModel(BBStr* modelpath)
{
    Model* model = new Model(modelpath->c_str());
    return model;
    delete modelpath;
}

void DrawModel(Model* model, float x, float y, float z)
{
    model->modelShader.Use();
    model->modelShader.SetMatrix("projection", PerspectiveMatrixRH(AngularToRadian(45.0f), (float)gx_runtime->GetWidth() / (float)gx_runtime->GetHeight(), 0.1f, 100.0f));
    model->modelShader.SetMatrix("view", ViewMatrixRH(VECTOR3(0.0f, 0.0f, 1.0f), VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f)));
    model->modelShader.SetMatrix("model", TranslationMatrix(x, y, z) * ScaleMatrix(0.001f, 0.001f, 0.001f));
    model->Draw();
}

void FreeModel(Model* model)
{
    model->Free();
    delete model;
}

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
    rtSym("%LoadModel$model", LoadModel);
    rtSym("DrawModel%model#x#y#z", DrawModel);
    rtSym("FreeModel%model", FreeModel);

    rtSym("%CreateAnimation%model", CreateAnimation);
    rtSym("PlayAnimation%anim#currTime%mode#speed%begin%end", PlayAnimation);
    rtSym("FreeAnimation%anim", FreeAnimation);
}