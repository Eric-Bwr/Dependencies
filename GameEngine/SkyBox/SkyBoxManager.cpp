#include "SkyBoxManager.h"

void SkyBoxManager::init(const char* shaderPath) {
    shader = new Shader(shaderPath);
    shader->addUniforms({"projection", "view"});
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    const float vertices[108] = {
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f
    };
    VertexBufferObjectLayout layout;
    layout.pushFloat(3);
    vbo.init(vertices, 36 * layout.getStride(), GL_STATIC_DRAW);
    vao.init();
    vao.addBuffer(vbo, layout);
    skyBox = nullptr;
}

void SkyBoxManager::render(Mat4f& view) {
    if(skyBox != nullptr) {
        shader->bind();
        vao.bind();
        glDepthFunc(GL_LEQUAL);
        skyBox->texture->bind();
        auto matrix = view;
        matrix.removeTranslation();
        matrix.rotate(skyBox->skyBoxRotation, skyBox->rotationX, skyBox->rotationY, skyBox->rotationZ);
        shader->setUniformMatrix4f("view", matrix.getBuffer());
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);
    }
}

void SkyBoxManager::set(SkyBox *skyBox) {
    this->skyBox = skyBox;
}

void SkyBoxManager::setProjection(Mat4f &projection) {
    shader->bind();
    shader->setUniformMatrix4f("projection", projection.getBuffer());
}

SkyBoxManager::~SkyBoxManager() {
    delete shader;
}
