#pragma once

#include <Math/Matrix.h>
#include <OpenGL/Texture.h>

class SkyBox {
public:
    void init(const char* path, const char* ending);
    void update(float skyBoxRotation, float rotationX, float rotationY, float rotationZ);
    ~SkyBox();
    float skyBoxRotation = 0.0f, rotationX = 0.0f, rotationY = 0.0f, rotationZ = 0.0f;
    Texture* texture;
};