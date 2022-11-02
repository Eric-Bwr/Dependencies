#pragma once

#include <OpenGL/Shader.h>
#include <OpenGL/Buffer.h>
#include "SkyBox.h"

class SkyBoxManager {
public:
    void init(const char* shaderPath);
    void render(Mat4f& view);
    void set(SkyBox* skyBox);
    void setProjection(Mat4f& projection);
    ~SkyBoxManager();
private:
    SkyBox* skyBox;
    Shader* shader;
    VertexArrayObject vao;
    VertexBufferObject vbo;
};