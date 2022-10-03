#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <Math/Vector.h>
#include <Math/Matrix.h>
#include <Math/Quaternion.h>

struct ResourceMaterialData{
    Vec3f diffuse;
    Vec3f specular;
    Vec3f emissive;
    Vec3f ambient;
    float shininess;
};

struct ResourceTextureData{
    std::vector<std::string> paths;
};

struct ResourceMeshData{
    Mat4f transformation;
    unsigned int materialIndex;
    std::vector<float> vertices;
    std::vector<float> textureCoords;
    std::vector<float> normals;
    std::vector<float> tangents;
    std::vector<float> bitangents;
    std::vector<float> boneIDs;
    std::vector<float> weights;
    std::vector<unsigned int> indices;
};

struct ResourceAssimpNodeData {
    std::string name;
    Mat4f transformation;
    std::vector<ResourceAssimpNodeData> children;
};

struct ResourceBoneData {
    unsigned int boneID;
    std::string name;
    Mat4f nodeTransform = identityMatrix();
    Mat4f offset = identityMatrix();
};

struct ResourceAnimationData {
    float duration = 0.0;
    float ticksPerSecond = 0.0;
    std::unordered_map<std::string, std::map<float, Vec3f>> keyPositions;
    std::unordered_map<std::string, std::map<float, Quatf>> keyRotations;
    std::unordered_map<std::string, std::map<float, Vec3f>> keyScales;
    ResourceAssimpNodeData nodeHierachy;
};

struct ResourceSceneData {
    ResourceAssimpNodeData node;
    std::vector<ResourceMeshData> meshes;
    std::vector<ResourceMaterialData> materials;
    std::vector<ResourceTextureData> textures;
    std::unordered_map<std::string, ResourceAnimationData> animations;
    std::unordered_map<std::string, std::unordered_map<std::string, ResourceBoneData>> skeletons;
};