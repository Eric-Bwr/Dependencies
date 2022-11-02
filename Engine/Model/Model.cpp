#include "Model.h"

Model::Model(const char *path, const char *textureFolderPath, unsigned int drawType, unsigned int drawMode)
        : textureFolderPath(textureFolderPath), drawType(drawType), drawMode(drawMode) {
    std::string newPath = path;
    newPath.append(".bmf");
    Binary_buffer binaryBuffer = {};
    binary_buffer_read_from_file(&binaryBuffer, newPath.c_str());
    ModelLoader::readResourceSceneData(resourceSceneData, &binaryBuffer);
    binary_buffer_destroy(&binaryBuffer);
}

void Model::load() {
    loadMaterials();
    for (auto &meshData: resourceSceneData.meshes) {
        std::vector<float> data;
        data.resize((meshData.vertices.size() / 3) * 14);
        for (auto i = 0; i < meshData.vertices.size() / 3; i++) {
            auto tri = i * 3;
            auto list = i * 14;
            auto duo = i * 2;
            data[list + 0] = meshData.vertices[tri + 0];
            data[list + 1] = meshData.vertices[tri + 1];
            data[list + 2] = meshData.vertices[tri + 2];
            data[list + 3] = meshData.textureCoords[duo + 0];
            data[list + 4] = meshData.textureCoords[duo + 1];
            data[list + 5] = meshData.normals[tri + 0];
            data[list + 6] = meshData.normals[tri + 1];
            data[list + 7] = meshData.normals[tri + 2];
            data[list + 8] = meshData.tangents[tri + 0];
            data[list + 9] = meshData.tangents[tri + 1];
            data[list + 10] = meshData.tangents[tri + 2];
            data[list + 11] = meshData.bitangents[tri + 0];
            data[list + 12] = meshData.bitangents[tri + 1];
            data[list + 13] = meshData.bitangents[tri + 2];
        }
        meshes.resize(meshes.size() + 1);
        auto mesh = meshes.back();
        mesh.init(data.data(), data.size(), meshData.indices.data(), meshData.indices.size(), false, drawType, drawMode);
        mesh.setMaterial(materials[meshData.materialIndex]);
    }
}

void Model::loadSkeletal() {
    loadMaterials();
    for (auto &meshData: resourceSceneData.meshes) {
        std::vector<float> data;
        data.resize((meshData.vertices.size() / 3) * 22);
        for (auto i = 0; i < meshData.vertices.size() / 3; i++) {
            auto duo = i * 2;
            auto tri = i * 3;
            auto quad = i * 4;
            auto list = i * 22;
            data[list + 0] = (meshData.vertices[tri + 0]);
            data[list + 1] = (meshData.vertices[tri + 1]);
            data[list + 2] = (meshData.vertices[tri + 2]);
            data[list + 3] = (meshData.textureCoords[duo + 0]);
            data[list + 4] = (meshData.textureCoords[duo + 1]);
            data[list + 5] = (meshData.normals[tri + 0]);
            data[list + 6] = (meshData.normals[tri + 1]);
            data[list + 7] = (meshData.normals[tri + 2]);
            data[list + 8] = (meshData.tangents[tri + 0]);
            data[list + 9] = (meshData.tangents[tri + 1]);
            data[list + 10] = (meshData.tangents[tri + 2]);
            data[list + 11] = (meshData.bitangents[tri + 0]);
            data[list + 12] = (meshData.bitangents[tri + 1]);
            data[list + 13] = (meshData.bitangents[tri + 2]);
            data[list + 14] = (meshData.boneIDs[quad + 0]);
            data[list + 15] = (meshData.boneIDs[quad + 1]);
            data[list + 16] = (meshData.boneIDs[quad + 2]);
            data[list + 17] = (meshData.boneIDs[quad + 3]);
            data[list + 18] = (meshData.weights[quad + 0]);
            data[list + 19] = (meshData.weights[quad + 1]);
            data[list + 20] = (meshData.weights[quad + 2]);
            data[list + 21] = (meshData.weights[quad + 3]);
        }
        meshes.resize(meshes.size() + 1);
        auto mesh = meshes.back();
        mesh.init(data.data(), data.size(), meshData.indices.data(), meshData.indices.size(), true, drawType, drawMode);
        mesh.setMaterial(materials[meshData.materialIndex]);
    }
}

void Model::loadMaterials() {
    for (auto &materialData: resourceSceneData.materials) {
        auto material = new Material;
        material->diffuseX = materialData.diffuse.x;
        material->diffuseY = materialData.diffuse.y;
        material->diffuseZ = materialData.diffuse.z;
        material->specularX = materialData.specular.x;
        material->specularY = materialData.specular.y;
        material->specularZ = materialData.specular.z;
        material->emissiveX = materialData.emissive.x;
        material->emissiveY = materialData.emissive.y;
        material->emissiveZ = materialData.emissive.z;
        material->ambientX = materialData.ambient.x;
        material->ambientY = materialData.ambient.y;
        material->ambientZ = materialData.ambient.z;
        material->shininess = materialData.shininess;
        for (auto &texture: material->textures)
            texture = nullptr;
        materials.push_back(material);
    }
    for (int i = 0; i < resourceSceneData.textures.size(); i++)
        for (int j = 0; j < resourceSceneData.textures[i].paths.size(); j++)
            materials[i]->textures[j] = nullptr;
}

void Model::loadTextures(const std::vector<std::string> &ignoreTextures) {
    for (int i = 0; i < resourceSceneData.textures.size(); i++) {
        for (int j = 0; j < resourceSceneData.textures[i].paths.size(); j++) {
            auto path = resourceSceneData.textures[i].paths[j];
            bool load = true;
            for (const auto &ignore: ignoreTextures) {
                if (path == ignore) {
                    load = false;
                    break;
                }
            }
            if (load)
                materials[i]->textures[j] = ModelLoader::getTexture(textureFolderPath, path.data());
        }
    }
}

void Model::setTexture(int materialIndex, int type, const char *path) {
    materials[materialIndex]->textures[type] = ModelLoader::getTexture(textureFolderPath, path);
}

void Model::setupMaterial(Shader *shader) {
    shader->addUniforms({"materialColorDiffuse", "materialColorSpecular", "materialColorEmissive", "materialColorAmbient", "materialShininess"});
    shader->addUniforms({"materialTextureDiffuse",
                         "materialTextureSpecular",
                         "materialTextureAmbient",
                         "materialTextureEmissive",
                         "materialTextureHeight",
                         "materialTextureNormal",
                         "materialTextureShininess",
                         "materialTextureOpacity",
                         "materialTextureDisplacement",
                         "materialTextureLightmap",
                         "materialTextureReflection"});
    shader->setUniformLocation("materialTextureDiffuse", 0);
    shader->setUniformLocation("materialTextureSpecular", 1);
    shader->setUniformLocation("materialTextureAmbient", 2);
    shader->setUniformLocation("materialTextureEmissive", 3);
    shader->setUniformLocation("materialTextureHeight", 4);
    shader->setUniformLocation("materialTextureNormal", 5);
    shader->setUniformLocation("materialTextureShininess", 6);
    shader->setUniformLocation("materialTextureOpacity", 7);
    shader->setUniformLocation("materialTextureDisplacement", 8);
    shader->setUniformLocation("materialTextureLightmap", 9);
    shader->setUniformLocation("materialTextureReflection", 10);
}

void Model::bindArrays() {
    for (auto mesh: meshes)
        mesh.bindArrays();
}

void Model::bind() {
    for (auto mesh: meshes)
        mesh.bind();
}

void Model::drawArrays() {
    for (auto mesh: meshes)
        mesh.drawArrays();
}

void Model::draw() {
    for (auto mesh: meshes)
        mesh.draw();
}

void Model::renderArrays() {
    for (auto mesh: meshes)
        mesh.renderArrays();
}

void Model::render() {
    for (auto mesh: meshes)
        mesh.render();
}

void Model::renderArraysMaterial(Shader *shader) {
    for (auto mesh: meshes) {
        mesh.bindMaterial(shader);
        mesh.renderArrays();
    }
}

void Model::renderMaterial(Shader *shader) {
    for (auto mesh: meshes) {
        mesh.bindMaterial(shader);
        mesh.render();
    }
}

Mesh *Model::getMesh(int index) {
    return &meshes[index];
}

static std::string convertTextureType(int type) {
    switch (type) {
        case MaterialTextureTypes::DIFFUSE:
            return "Diffuse";
        case MaterialTextureTypes::SPECULAR:
            return "Specular";
        case MaterialTextureTypes::AMBIENT:
            return "Ambient";
        case MaterialTextureTypes::EMISSIVE:
            return "Emissive";
        case MaterialTextureTypes::HEIGHT:
            return "Height";
        case MaterialTextureTypes::NORMAL:
            return "Normal";
        case MaterialTextureTypes::SHININESS:
            return "Shininess";
        case MaterialTextureTypes::OPACITY:
            return "Opacity";
        case MaterialTextureTypes::DISPLACEMENT:
            return "Displacement";
        case MaterialTextureTypes::LIGHTMAP:
            return "Lightmap";
        case MaterialTextureTypes::REFLECTION:
            return "Reflection";
        default:
            return "Unknown";
    }
}

std::string Model::texturesToString() {
    std::string result("Textures:\n");
    for (int i = 0; i < resourceSceneData.textures.size(); i++) {
        result += "Material: " + std::to_string(i) + "\n";
        for (int j = 0; j < resourceSceneData.textures[i].paths.size(); j++) {
            auto path = resourceSceneData.textures[i].paths[j];
            if (!path.empty())
                result += convertTextureType(j) + ": " + path + "\n";
        }
    }
    return result;
}

std::string Model::animationsToString() {
    std::string result("Animations:\n");
    for (auto &animation: resourceSceneData.animations)
        result += animation.first + "\n";
    return result;
}

std::string Model::skeletonsToString() {
    std::string result("Skeletons:\n");
    for (auto &skeleton: resourceSceneData.skeletons)
        result += skeleton.first + "\n";
    return result;
}

ResourceAnimationData *Model::getAnimation(const std::string &name) {
    return &resourceSceneData.animations.find(name)->second;
}

std::unordered_map<std::string, ResourceBoneData> *Model::getSkeleton(const std::string &name) {
    return &resourceSceneData.skeletons.find(name)->second;
}

Model::~Model() {
    meshes.clear();
    for (auto material : materials)
        delete material;
}