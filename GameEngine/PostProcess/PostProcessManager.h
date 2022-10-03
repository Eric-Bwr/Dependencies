#pragma once

#include <OpenGL/Shader.h>
#include <OpenGL/Buffer.h>
#include <OpenGL/FrameBuffer.h>

class PostProcessManager {
public:
    void init(int width, int height);
    void bind();
    void unbind();
    void processEffects();
    void drawEffects();
    void process();
    void setTime(float time);
    void shake(float strength = 0.0f, float intensity = 0.0f);
    void contrast(float contrast = 0.0f);
    void grayscale(bool grayscale = false);
    void gaussianBlur(float divisor = 0.0f);
    void setSize(int width, int height);
    bool hasErrors();
    std::string getErrorMessage();
    ~PostProcessManager();
private:
    FrameBuffer* frameBuffer;
    RenderBuffer* renderBuffer;
    Shader* shader;
    Shader* blurVerticalShader;
    Shader* blurHorizontalShader;
    VertexArrayObject vao;
    VertexBufferObject vbo;
    Texture* colorBuffer;
    Texture* blurVColorBuffer;
    Texture* blurHColorBuffer;
    int width, height;
    float time = 0.0f, divisor = 0.0f;
};