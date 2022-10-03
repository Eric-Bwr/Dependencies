#pragma once

#include <OpenGL/Buffer.h>
#include <OpenGL/Texture.h>
#include "FileStream.h"
#include "ModelStructure.h"

enum MaterialTextureTypes {
    DIFFUSE = 0,
    SPECULAR,
    AMBIENT,
    EMISSIVE,
    HEIGHT,
    NORMAL,
    SHININESS,
    OPACITY,
    DISPLACEMENT,
    LIGHTMAP,
    REFLECTION
};

struct Material {
    float diffuseX, diffuseY, diffuseZ,
          specularX, specularY, specularZ,
          emissiveX, emissiveY, emissiveZ,
          ambientX, ambientY, ambientZ,
          shininess;
    Texture* textures[11];
};

class ModelLoader{
public:
    static void init(unsigned int format = GL_RGBA, unsigned int internalFormat = GL_RGBA16);
    static void readResourceSceneData(ResourceSceneData& resourceSceneData, Binary_buffer* binaryBuffer);
    static Texture* getTexture(const std::string& textureFolderPath, const char* path);
    static VertexBufferObjectLayout layout;
    static VertexBufferObjectLayout skeletalLayout;
    static unsigned int ssbo;
    ~ModelLoader();
private:
    static void readNode(ResourceAssimpNodeData& node, Binary_buffer* binaryBuffer);
    static void readAnimationMaps(std::unordered_map<std::string, std::map<float, Vec3f>>& map, Binary_buffer* binaryBuffer);
    static void readAnimationMaps(std::unordered_map<std::string, std::map<float, Quatf>>& map, Binary_buffer* binaryBuffer);
    static void readArray(std::vector<float>& array, Binary_buffer* binaryBuffer);
    static void readArray(std::vector<unsigned int>& array, Binary_buffer* binaryBuffer);
    static void readString(std::string& string, Binary_buffer* binaryBuffer);
    static void readVec3(Vec3f& vector, Binary_buffer* binaryBuffer);
    static void readQuat(Quatf& quat, Binary_buffer* binaryBuffer);
    static void readMatrix(Mat4f& matrix, Binary_buffer *binaryBuffer);
};

