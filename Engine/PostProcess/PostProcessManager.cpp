#include "PostProcessManager.h"
#include "PostShader.h"

void PostProcessManager::init(int width, int height) {
    this->width = width;
    this->height = height;
    frameBuffer = new FrameBuffer();
    colorBuffer = new Texture();
    colorBuffer->setWidth(width);
    colorBuffer->setHeight(height);
    colorBuffer->load();
    colorBuffer->minLinear();
    colorBuffer->magLinear();
    colorBuffer->clampEdge();
    frameBuffer->attachColorBuffer(*colorBuffer, GL_COLOR_ATTACHMENT0);
    blurVColorBuffer = new Texture();
    blurVColorBuffer->bind();
    blurVColorBuffer->setWidth(width);
    blurVColorBuffer->setHeight(height);
    blurVColorBuffer->load();
    blurVColorBuffer->minLinear();
    blurVColorBuffer->magLinear();
    blurVColorBuffer->clampEdge();
    frameBuffer->attachColorBuffer(*blurVColorBuffer, GL_COLOR_ATTACHMENT1);
    blurHColorBuffer = new Texture();
    blurHColorBuffer->bind();
    blurHColorBuffer->setWidth(width);
    blurHColorBuffer->setHeight(height);
    blurHColorBuffer->load();
    blurHColorBuffer->minLinear();
    blurHColorBuffer->magLinear();
    blurHColorBuffer->clampEdge();
    frameBuffer->attachColorBuffer(*blurHColorBuffer, GL_COLOR_ATTACHMENT2);
    renderBuffer = new RenderBuffer(GL_DEPTH_COMPONENT32F, width, height);
    frameBuffer->attachRenderBuffer(*renderBuffer, GL_DEPTH_ATTACHMENT);
    shader = new Shader();
    shader->addVertexShader(PostShaderVertex);
    shader->addFragmentShader(PostShaderFragment);
    shader->finish();
    shader->addUniforms({"time", "strength", "intensity", "contrast", "hasGrayscale"});
    blurHorizontalShader = new Shader();
    blurHorizontalShader->addVertexShader(PostShaderHBlurVertex);
    blurHorizontalShader->addFragmentShader(PostShaderBlurFragment);
    blurHorizontalShader->finish();
    blurHorizontalShader->addUniform("targetFrameBufferWidth");
    blurVerticalShader = new Shader();
    blurVerticalShader->addVertexShader(PostShaderVBlurVertex);
    blurVerticalShader->addFragmentShader(PostShaderBlurFragment);
    blurVerticalShader->finish();
    blurVerticalShader->addUniform("targetFrameBufferHeight");
    float vertices[8] = {-1, 1, -1, -1, 1, 1, 1, -1};
    vao.init();
    VertexBufferObjectLayout layout;
    layout.pushFloat(2),
    vbo.init(vertices, layout.getStride() * 4, GL_STATIC_DRAW);
    vao.addBuffer(vbo, layout);
    frameBuffer->finish();
    frameBuffer->unbind();
}

void PostProcessManager::bind() {
    frameBuffer->bind();
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void PostProcessManager::unbind() {
    frameBuffer->unbind();
}

void PostProcessManager::processEffects() {
    glDisable(GL_DEPTH_TEST);
    if (divisor != 0.0f) {
        glDrawBuffer(GL_COLOR_ATTACHMENT1);
        glClear(GL_COLOR_BUFFER_BIT);
        blurVerticalShader->bind();
        vao.bind();
        colorBuffer->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawBuffer(GL_COLOR_ATTACHMENT2);
        glClear(GL_COLOR_BUFFER_BIT);
        blurHorizontalShader->bind();
        vao.bind();
        blurVColorBuffer->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    frameBuffer->unbind();
    shader->bind();
    vao.bind();
    if (divisor != 0.0f)
        blurHColorBuffer->bind();
    else
        colorBuffer->bind();
    shader->setUniform1f("time", time);
}

void PostProcessManager::drawEffects() {
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void PostProcessManager::process() {
    glDisable(GL_DEPTH_TEST);
    if (divisor != 0.0f) {
        glDrawBuffer(GL_COLOR_ATTACHMENT1);
        glClear(GL_COLOR_BUFFER_BIT);
        blurVerticalShader->bind();
        vao.bind();
        colorBuffer->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawBuffer(GL_COLOR_ATTACHMENT2);
        glClear(GL_COLOR_BUFFER_BIT);
        blurHorizontalShader->bind();
        vao.bind();
        blurVColorBuffer->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    frameBuffer->unbind();
    shader->bind();
    vao.bind();
    if (divisor != 0.0f)
        blurHColorBuffer->bind();
    else
        colorBuffer->bind();
    shader->setUniform1f("time", time);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void PostProcessManager::setTime(float time) {
    this->time = time;
}

void PostProcessManager::shake(float strength, float intensity) {
    shader->bind();
    shader->setUniform1f("strength", strength);
    shader->setUniform1f("intensity", intensity);
}

void PostProcessManager::contrast(float contrast) {
    shader->setUniform1f("contrast", contrast);
}

void PostProcessManager::grayscale(bool grayscale) {
    shader->setUniformBool("hasGrayscale", grayscale);
}

void PostProcessManager::gaussianBlur(float divisor) {
    this->divisor = divisor;
    blurHorizontalShader->bind();
    blurHorizontalShader->setUniform1f("targetFrameBufferWidth", width / divisor);
    blurVerticalShader->bind();
    blurVerticalShader->setUniform1f("targetFrameBufferHeight", height / divisor);
}

void PostProcessManager::setSize(int width, int height) {
    this->width = width;
    this->height = height;
    frameBuffer->resize(width, height);
    blurHorizontalShader->bind();
    blurHorizontalShader->setUniform1f("targetFrameBufferWidth", width / divisor);
    blurVerticalShader->bind();
    blurVerticalShader->setUniform1f("targetFrameBufferHeight", height / divisor);
    colorBuffer->resize(width, height);
    blurVColorBuffer->resize(width, height);
    blurHColorBuffer->resize(width, height);
}

bool PostProcessManager::hasErrors() {
    return shader->hasError() || blurHorizontalShader->hasError() || blurVerticalShader->hasError() || colorBuffer->hasError() || blurVColorBuffer->hasError() || blurHColorBuffer->hasError();
}

std::string PostProcessManager::getErrorMessage() {
    std::string result;
    if(hasErrors()){
        result.append("PostProcessManager: \n");
        if(shader->hasError())
            result.append(shader->getErrorMessage().c_str());
        if(blurHorizontalShader->hasError())
            result.append(blurHorizontalShader->getErrorMessage().c_str());
        if(blurVerticalShader->hasError())
            result.append(blurVerticalShader->getErrorMessage().c_str());
        if(colorBuffer->hasError())
            result.append(colorBuffer->getErrorMessage().c_str());
        if(blurVColorBuffer->hasError())
            result.append(blurVColorBuffer->getErrorMessage().c_str());
        if(blurHColorBuffer->hasError())
            result.append(blurHColorBuffer->getErrorMessage().c_str());
        result.append("\n");
    }
    return result;
}

PostProcessManager::~PostProcessManager() {
    delete frameBuffer;
    delete shader;
    delete blurVerticalShader;
    delete blurHorizontalShader;
    delete renderBuffer;
    delete colorBuffer;
    delete blurVColorBuffer;
    delete blurHColorBuffer;
}