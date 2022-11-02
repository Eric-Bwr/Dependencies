#pragma once

#include "Mesh.h"

class Model {
public:
    explicit Model(const char* path, const char* textureFolderPath = "", unsigned int drawType = GL_TRIANGLES, unsigned int drawMode = GL_STATIC_DRAW);
    void load();
    void loadSkeletal();
    void loadTextures(const std::vector<std::string>& ignoreTextures = {});
    void setTexture(int materialIndex, int type, const char* path);
    void setupMaterial(Shader* shader);
    void bindArrays();
    void bind();
    void drawArrays();
    void draw();
    void renderArrays();
    void render();
    void renderArraysMaterial(Shader* shader);
    void renderMaterial(Shader* shader);
    Mesh* getMesh(int index);
    std::string texturesToString();
    std::string animationsToString();
    std::string skeletonsToString();
    ResourceAnimationData* getAnimation(const std::string& name);
    std::unordered_map<std::string, ResourceBoneData>* getSkeleton(const std::string& name);
    ~Model();
private:
    ResourceSceneData resourceSceneData;
    std::vector<Material*> materials;
    std::vector<Mesh> meshes;
    std::string textureFolderPath;
    unsigned int drawType, drawMode;
    void loadMaterials();
};
