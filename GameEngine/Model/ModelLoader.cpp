#include <cstring>
#include "ModelLoader.h"
#include "OpenGL/GLUtil/STB.h"

static unsigned int format, internalFormat;
static std::vector<Texture*> textures;
VertexBufferObjectLayout ModelLoader::layout;
VertexBufferObjectLayout ModelLoader::skeletalLayout;
unsigned int ModelLoader::ssbo;

void ModelLoader::init(unsigned int _format, unsigned int _internalFormat){
    format = _format;
    internalFormat = _internalFormat;
    layout.pushFloat(3);
    layout.pushFloat(2);
    layout.pushFloat(3);
    layout.pushFloat(3);
    layout.pushFloat(3);

    skeletalLayout.pushFloat(3);
    skeletalLayout.pushFloat(2);
    skeletalLayout.pushFloat(3);
    skeletalLayout.pushFloat(3);
    skeletalLayout.pushFloat(3);
    skeletalLayout.pushFloat(4);
    skeletalLayout.pushFloat(4);

    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
}

void ModelLoader::readResourceSceneData(ResourceSceneData &resourceSceneData, Binary_buffer *binaryBuffer) {
    readNode(resourceSceneData.node, binaryBuffer);
    uint32_t numMaterials = binary_buffer_pop32(binaryBuffer);
    uint32_t numTextures = binary_buffer_pop32(binaryBuffer);
    uint32_t numMeshes = binary_buffer_pop32(binaryBuffer);
    uint32_t numAnimations = binary_buffer_pop32(binaryBuffer);
    uint32_t numSkeletons = binary_buffer_pop32(binaryBuffer);
    resourceSceneData.materials.resize(numMaterials);
    resourceSceneData.textures.resize(numTextures);
    resourceSceneData.meshes.resize(numMeshes);
    for(auto& material : resourceSceneData.materials){
        readVec3(material.diffuse, binaryBuffer);
        readVec3(material.specular, binaryBuffer);
        readVec3(material.emissive, binaryBuffer);
        readVec3(material.ambient, binaryBuffer);
        material.shininess = binary_buffer_pop_float(binaryBuffer);
    }
    for(auto& texture : resourceSceneData.textures){
        auto size = binary_buffer_pop32(binaryBuffer);
        texture.paths.resize(size);
        for(auto i = 0; i < size; i++)
            readString(texture.paths[i], binaryBuffer);
    }
    for(auto& mesh : resourceSceneData.meshes){
        readMatrix(mesh.transformation, binaryBuffer);
        mesh.materialIndex = binary_buffer_pop32(binaryBuffer);
        readArray(mesh.vertices, binaryBuffer);
        readArray(mesh.textureCoords, binaryBuffer);
        readArray(mesh.normals, binaryBuffer);
        readArray(mesh.tangents, binaryBuffer);
        readArray(mesh.bitangents, binaryBuffer);
        readArray(mesh.boneIDs, binaryBuffer);
        readArray(mesh.weights, binaryBuffer);
        readArray(mesh.indices, binaryBuffer);
    }
    std::string name;
    for(auto i = 0; i < numAnimations; i++){
        readString(name, binaryBuffer);
        ResourceAnimationData animationData;
        animationData.duration = binary_buffer_pop_float(binaryBuffer);
        animationData.ticksPerSecond = binary_buffer_pop_float(binaryBuffer);
        readNode(animationData.nodeHierachy, binaryBuffer);
        readAnimationMaps(animationData.keyPositions, binaryBuffer);
        readAnimationMaps(animationData.keyRotations, binaryBuffer);
        readAnimationMaps(animationData.keyScales, binaryBuffer);
        resourceSceneData.animations[name] = animationData;
    }
    for(auto i = 0; i < numSkeletons; i++){
        readString(name, binaryBuffer);
        auto size = binary_buffer_pop64(binaryBuffer);
        std::string nestedKey;
        for(auto j = 0; j < size; j++){
            readString(nestedKey, binaryBuffer);
            ResourceBoneData boneData;
            boneData.name = nestedKey;
            boneData.boneID = binary_buffer_pop64(binaryBuffer);
            readMatrix(boneData.nodeTransform, binaryBuffer);
            readMatrix(boneData.offset, binaryBuffer);
            resourceSceneData.skeletons[name][nestedKey] = boneData;
        }
    }
}

void ModelLoader::readNode(ResourceAssimpNodeData& node, Binary_buffer* binaryBuffer){
    readString(node.name, binaryBuffer);
    auto name = node.name;
    readMatrix(node.transformation, binaryBuffer);
    auto size = binary_buffer_pop32(binaryBuffer);
    node.children.resize(size);
    for(auto& child : node.children)
        readNode(child, binaryBuffer);
}

void ModelLoader::readAnimationMaps(std::unordered_map<std::string, std::map<float, Vec3f>>& map, Binary_buffer* binaryBuffer){
    auto size = binary_buffer_pop64(binaryBuffer);
    std::string key;
    Vec3f nestedValue;
    for(auto i = 0; i < size; i++) {
        auto nestedMapSize = binary_buffer_pop64(binaryBuffer);
        readString(key, binaryBuffer);
        for (auto j = 0; j < nestedMapSize; j++) {
            auto nestedKey = binary_buffer_pop_float(binaryBuffer);
            readVec3(nestedValue, binaryBuffer);
            map[key][nestedKey] = nestedValue;
        }
    }
}

void ModelLoader::readAnimationMaps(std::unordered_map<std::string, std::map<float, Quatf>>& map, Binary_buffer* binaryBuffer){
    auto size = binary_buffer_pop64(binaryBuffer);
    std::string key;
    Quatf nestedValue;
    for(auto i = 0; i < size; i++) {
        auto nestedMapSize = binary_buffer_pop64(binaryBuffer);
        readString(key, binaryBuffer);
        for (auto j = 0; j < nestedMapSize; j++) {
            auto nestedKey = binary_buffer_pop_float(binaryBuffer);
            readQuat(nestedValue, binaryBuffer);
            map[key][nestedKey] = nestedValue;
        }
    }
}

void ModelLoader::readArray(std::vector<float>& array, Binary_buffer* binaryBuffer){
    array.resize(binary_buffer_pop64(binaryBuffer));
    for(auto& element : array)
        element = binary_buffer_pop_float(binaryBuffer);
}

void ModelLoader::readArray(std::vector<unsigned int>& array, Binary_buffer* binaryBuffer){
    array.resize(binary_buffer_pop64(binaryBuffer));
    for(auto& element : array)
        element = binary_buffer_pop64(binaryBuffer);
}

void ModelLoader::readString(std::string& string, Binary_buffer* binaryBuffer){
    auto size = binary_buffer_pop32(binaryBuffer);
    char* data = (char*)binary_buffer_pop_string_with_termination(binaryBuffer, size);
    string = data;
    free(data);
}

void ModelLoader::readVec3(Vec3f& vector, Binary_buffer* binaryBuffer){
    vector.x = binary_buffer_pop_float(binaryBuffer);
    vector.y = binary_buffer_pop_float(binaryBuffer);
    vector.z = binary_buffer_pop_float(binaryBuffer);
}

void ModelLoader::readQuat(Quatf& quat, Binary_buffer* binaryBuffer){
    quat.x = binary_buffer_pop_float(binaryBuffer);
    quat.y = binary_buffer_pop_float(binaryBuffer);
    quat.z = binary_buffer_pop_float(binaryBuffer);
    quat.w = binary_buffer_pop_float(binaryBuffer);
}

void ModelLoader::readMatrix(Mat4f& matrix, Binary_buffer *binaryBuffer) {
    matrix.m00 = binary_buffer_pop_float(binaryBuffer);
    matrix.m01 = binary_buffer_pop_float(binaryBuffer);
    matrix.m02 = binary_buffer_pop_float(binaryBuffer);
    matrix.m03 = binary_buffer_pop_float(binaryBuffer);

    matrix.m10 = binary_buffer_pop_float(binaryBuffer);
    matrix.m11 = binary_buffer_pop_float(binaryBuffer);
    matrix.m12 = binary_buffer_pop_float(binaryBuffer);
    matrix.m13 = binary_buffer_pop_float(binaryBuffer);

    matrix.m20 = binary_buffer_pop_float(binaryBuffer);
    matrix.m21 = binary_buffer_pop_float(binaryBuffer);
    matrix.m22 = binary_buffer_pop_float(binaryBuffer);
    matrix.m23 = binary_buffer_pop_float(binaryBuffer);

    matrix.m30 = binary_buffer_pop_float(binaryBuffer);
    matrix.m31 = binary_buffer_pop_float(binaryBuffer);
    matrix.m32 = binary_buffer_pop_float(binaryBuffer);
    matrix.m33 = binary_buffer_pop_float(binaryBuffer);
}

Texture *ModelLoader::getTexture(const std::string& textureFolderPath, const char* path) {
    if(strcmp(path, "") == 0)
        return nullptr;
    auto combinedPath = (textureFolderPath + path);
    for (auto texture : textures) {
        if (texture->getPath() == combinedPath)
            return texture;
    }
    auto texture = new Texture();
    texture->setPath(combinedPath.data());
    int width, height, nrComponents;
    uint8_t *data = stbi_load(combinedPath.data(), &width, &height, &nrComponents, 4);
    texture->setFormat(format);
    texture->setInternalFormat(internalFormat);
    texture->setWidth(width);
    texture->setHeight(height);
    texture->setData(data);
    texture->load();
    texture->repeat();
    texture->minLinearMipLinear();
    texture->magLinear();
    texture->generateMipMap();
    textures.push_back(texture);
    return texture;
}

ModelLoader::~ModelLoader() {
    for(auto texture : textures)
        delete texture;
}