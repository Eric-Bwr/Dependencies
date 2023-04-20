#pragma once

#include <Math/Matrix.h>
#include "Controller.h"

class Camera : public Controller {
public:
    inline void setMouseSpeed(float mouseSpeed) { this->mouseSpeed = mouseSpeed; }
    inline void setPitch(float pitchMin, float pitchMax) { this->pitchMin = pitchMin; this->pitchMax = pitchMax; }
    inline Mat4d getViewMatrixd() { return viewMatrix; }
    inline Mat4f getViewMatrixf() { return convert<float>(viewMatrix); }
public:
    float mouseSpeed = 0.05;
    float pitchMin = -89.0f, pitchMax = 89.0f;
    bool hasLastPos = false;
    double lastPosX, lastPosY, xOffset, yOffset;
    Mat4d viewMatrix;
};