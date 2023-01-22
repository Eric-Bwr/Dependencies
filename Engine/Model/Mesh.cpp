#include "Mesh.h"

void Mesh::init(const float *vertices, uint64_t verticesSize, VertexBufferObjectLayout& layout, unsigned int drawType, unsigned int drawMode) {
    this->drawType = drawType;
    vao.init();
    unsigned int divisor = 0;
    for(auto element : layout.getElements())
        divisor += element.count;
    numVertices = verticesSize / divisor;
    vbo.init(vertices, layout.getStride() * numVertices, drawMode);
    vao.addBuffer(vbo, layout);
}

void Mesh::init(const float *vertices, uint64_t verticesSize, bool skeletal, unsigned int drawType, unsigned int drawMode) {
    this->drawType = drawType;
    vao.init();
    if(!skeletal) {
        numVertices = verticesSize / 14;
        vbo.init(vertices, ModelLoader::layout.getStride() * numVertices, drawMode);
        vao.addBuffer(vbo, ModelLoader::layout);
    } else {
        numVertices = verticesSize / 22;
        vbo.init(vertices, ModelLoader::skeletalLayout.getStride() * numVertices, drawMode);
        vao.addBuffer(vbo, ModelLoader::skeletalLayout);
    }
}

void Mesh::init(const float *vertices, uint64_t verticesSize, const unsigned int *indices, uint64_t indicesSize, bool skeletal, unsigned int drawType, unsigned int drawMode) {
    this->drawType = drawType;
    this->numIndices = indicesSize;
    vao.init();
    if(!skeletal) {
        numVertices = verticesSize / 14;
        vbo.init(vertices, ModelLoader::layout.getStride() * numVertices, drawMode);
        vao.addBuffer(vbo, ModelLoader::layout);
    } else {
        numVertices = verticesSize / 22;
        vbo.init(vertices, ModelLoader::skeletalLayout.getStride() * numVertices, drawMode);
        vao.addBuffer(vbo, ModelLoader::skeletalLayout);
    }
    ibo.init(indices, numIndices, drawMode);
}

void Mesh::changeVertices(const float *vertices, const uint64_t verticesSize, unsigned int offset, unsigned int drawMode) {
    vbo.subData(vertices, verticesSize, offset, drawMode);
}

void Mesh::changeIndices(const float *indices, const uint64_t indicesSize, unsigned int offset, unsigned int drawMode) {
    ibo.subData(indices, indicesSize, offset, drawMode);
}

void Mesh::setMaterial(Material* material) {
    this->material = material;
}

void Mesh::bindMaterialColors(Shader *shader) {
    shader->setUniform3f("materialColorDiffuse", material->diffuseX, material->diffuseY, material->diffuseZ);
    shader->setUniform3f("materialColorSpecular", material->specularX, material->specularY, material->specularZ);
    shader->setUniform3f("materialColorEmissive", material->emissiveX, material->emissiveY, material->emissiveZ);
    shader->setUniform3f("materialColorAmbient", material->ambientX, material->ambientY, material->ambientZ);
    shader->setUniform1f("materialShininess", material->shininess);
}

void Mesh::bindMaterialTextures() {
    for(int i = 0; i < 11; i++){
        auto texture = material->textures[i];
        if(texture != nullptr) {
            glActiveTexture(GL_TEXTURE0 + i);
            texture->bind();
        }
    }
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::bindMaterial(Shader *shader) {
    shader->setUniform3f("materialColorDiffuse", material->diffuseX, material->diffuseY, material->diffuseZ);
    shader->setUniform3f("materialColorSpecular", material->specularX, material->specularY, material->specularZ);
    shader->setUniform3f("materialColorEmissive", material->emissiveX, material->emissiveY, material->emissiveZ);
    shader->setUniform3f("materialColorAmbient", material->ambientX, material->ambientY, material->ambientZ);
    shader->setUniform1f("materialShininess", material->shininess);
    for(int i = 0; i < 11; i++){
        auto texture = material->textures[i];
        if(texture != nullptr) {
            glActiveTexture(GL_TEXTURE0 + i);
            texture->bind();
        }
    }
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::bindArrays() {
    vao.bind();
}

void Mesh::bind() {
    vao.bind();
    ibo.bind();
}

void Mesh::drawArrays() {
    glDrawArrays(drawType, 0, numVertices);
}

void Mesh::draw() {
    vao.bind();
    ibo.bind();
    glDrawElements(drawType, numIndices, GL_UNSIGNED_INT, nullptr);
}

void Mesh::renderArrays() {
    vao.bind();
    glDrawArrays(drawType, 0, numVertices);
}

void Mesh::renderArrays(int customVertices) {
    vao.bind();
    glDrawArrays(drawType, 0, customVertices);
}

void Mesh::render() {
    vao.bind();
    ibo.bind();
    glDrawElements(drawType, numIndices, GL_UNSIGNED_INT, nullptr);
}