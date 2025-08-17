#ifndef CAMERA_H
#define CAMERA_H

#include "MD_Math.h"
#include "Input.h"

class Camera
{
private:
    MD_Math::VECTOR3 cameraPos = {0.0f, 0.0f, 0.0f};
    MD_Math::VECTOR3 cameraFront = {0.0f, 0.0f, -1.0f};
    MD_Math::VECTOR3 cameraUp = {0.0f, 1.0f, 0.0f};
    
    MD_Math::QUATERNION orientation = MD_Math::QUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
    
    float pitchDelta = 0.0f;
    float yawDelta = 0.0f;
    
    const float MAX_PITCH = 89.0f;
    const float MIN_PITCH = -89.0f;
    float currentPitch = 0.0f;
    
public:
    Camera();
    ~Camera();

    void SetPos(MD_Math::VECTOR3 Pos)
    {
        this->cameraPos = Pos;
    }

    void Move(float cameraSpeed, float mouseSpeed)
    {
    
        if(Input_IsKeyDown(GLFW_KEY_W))
            cameraPos += cameraFront * cameraSpeed;

        if(Input_IsKeyDown(GLFW_KEY_S))
            cameraPos -= cameraFront * cameraSpeed;

        if(Input_IsKeyDown(GLFW_KEY_A)) {
            MD_Math::VECTOR3 right = MD_Math::Vector3Normalized(
                MD_Math::VectorCross(cameraFront, cameraUp)
            );
            cameraPos -= right * cameraSpeed;
        }

        if(Input_IsKeyDown(GLFW_KEY_D)) {
            MD_Math::VECTOR3 right = MD_Math::Vector3Normalized(
                MD_Math::VectorCross(cameraFront, cameraUp)
            );
            cameraPos += right * cameraSpeed;
        }
        
        pitchDelta = 0.0f;
        yawDelta = 0.0f;
        
        if(Input_IsKeyDown(GLFW_KEY_UP))
            pitchDelta += mouseSpeed;
        if(Input_IsKeyDown(GLFW_KEY_DOWN))
            pitchDelta -= mouseSpeed;
        if(Input_IsKeyDown(GLFW_KEY_LEFT))
            yawDelta += mouseSpeed;
        if(Input_IsKeyDown(GLFW_KEY_RIGHT))
            yawDelta -= mouseSpeed;
        
        if (pitchDelta != 0.0f || yawDelta != 0.0f) {
            ApplyRotation();
        }
    }

    void ApplyRotation()
    {

        if (yawDelta != 0.0f) {
            MD_Math::QUATERNION yawQuat = MD_Math::QuaternionFromAxisAngle(
                MD_Math::VECTOR3(0.0f, 1.0f, 0.0f),
                MD_Math::AngularToRadian(yawDelta)
            );
            
            orientation = yawQuat * orientation;
        }
    
        if (pitchDelta != 0.0f) {
            
            currentPitch += pitchDelta;
            if (currentPitch > MAX_PITCH) {
                pitchDelta -= (currentPitch - MAX_PITCH);
                currentPitch = MAX_PITCH;
            }
            if (currentPitch < MIN_PITCH) {
                pitchDelta -= (currentPitch - MIN_PITCH);
                currentPitch = MIN_PITCH;
            }
            
            MD_Math::VECTOR3 right = MD_Math::Vector3Normalized(
                MD_Math::VectorCross(cameraFront, cameraUp)
            );
            
            MD_Math::QUATERNION pitchQuat = MD_Math::QuaternionFromAxisAngle(
                right, 
                MD_Math::AngularToRadian(pitchDelta)
            );
            
            orientation = pitchQuat * orientation;
        }
        
        orientation = MD_Math::QuaternionNormalize(orientation);
        
        cameraFront = MD_Math::RotateVector(orientation, MD_Math::VECTOR3(0.0f, 0.0f, -1.0f));
        cameraFront = MD_Math::Vector3Normalized(cameraFront);
        
        MD_Math::VECTOR3 right = MD_Math::Vector3Normalized(
            MD_Math::VectorCross(cameraFront, MD_Math::VECTOR3(0.0f, 1.0f, 0.0f))
        );
        cameraUp = MD_Math::Vector3Normalized(
            MD_Math::VectorCross(right, cameraFront)
        );
    }

    MD_Math::MATRIX Matrix()
    {
        return MD_Math::ViewMatrixRH(
            cameraPos,
            cameraPos + cameraFront,
            cameraUp
        );
    }

    MD_Math::VECTOR3 Pos()
    {
        return cameraPos;
    }

    MD_Math::VECTOR3 Front()
    {
        return cameraFront;
    }
    
    float GetPitch() const { return currentPitch; }
};

Camera::Camera()
{
    orientation = MD_Math::QuaternionFromAxisAngle(
        MD_Math::VECTOR3(0.0f, 1.0f, 0.0f), 
        MD_Math::AngularToRadian(0.0f)
    );
    cameraFront = MD_Math::RotateVector(orientation, MD_Math::VECTOR3(0.0f, 0.0f, -1.0f));
}

Camera::~Camera()
{
}

#endif