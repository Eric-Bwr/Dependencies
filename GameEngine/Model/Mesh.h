#pragma once

#include <OpenGL/Shader.h>
#include "ModelLoader.h"

class Mesh {
public:
    void init(const float* vertices, uint64_t verticesSize, VertexBufferObjectLayout& layout, unsigned int drawType = GL_TRIANGLES, unsigned int drawMode = GL_STATIC_DRAW);
    void init(const float* vertices, uint64_t verticesSize, bool skeletal = false, unsigned int drawType = GL_TRIANGLES, unsigned int drawMode = GL_STATIC_DRAW);
    void init(const float* vertices, uint64_t verticesSize, const unsigned int* indices, uint64_t indicesSize, bool skeletal = false, unsigned int drawType = GL_TRIANGLES, unsigned int drawMode = GL_STATIC_DRAW);
    void changeVertices(const float* vertices, uint64_t verticesSize, unsigned int offset, unsigned int drawMode);
    void changeIndices(const float* indices, uint64_t indicesSize, unsigned int offset, unsigned int drawMode);
    void setMaterial(Material* material);
    void bindMaterialColors(Shader* shader);
    void bindMaterialTextures();
    void bindMaterial(Shader* shader);
    void bindArrays();
    void bind();
    void drawArrays();
    void draw();
    void renderArrays();
    void renderArrays(int customVertices);
    void render();
private:
    VertexArrayObject vao;
    VertexBufferObject vbo;
    IndicesBufferObject ibo;
    Material* material;
    uint64_t numIndices = 0;
    uint64_t numVertices = 0;
    unsigned int drawType = GL_TRIANGLES;
};