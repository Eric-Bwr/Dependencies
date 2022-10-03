#pragma once

#include <OpenGL/Shader.h>
#include "ModelStructure.h"

class ModelAnimator {
public:
    void playAnimation(ResourceAnimationData* animation, std::unordered_map<std::string, ResourceBoneData>* skeleton, float speed = 1.0f);
    void updateAnimation(double deltaFrameTime);
    void loadTransformations();
private:
    void calculateBoneTransform(const ResourceAssimpNodeData* node, Mat4f parentTransform);
    Vec3f interpolatePosition(const std::string& boneName);
    Quatf interpolateRotation(const std::string& boneName);
    Vec3f interpolateScaling(const std::string& boneName);
    void interpolateBone(Mat4f& matrix, const std::string& boneName);
    std::unordered_map<std::string, std::map<float, Vec3f>::iterator> mapPositionIterator;
    std::unordered_map<std::string, std::map<float, Quatf>::iterator> mapRotationIterator;
    std::unordered_map<std::string, std::map<float, Vec3f>::iterator> mapScalingIterator;
    std::unordered_map<std::string, ResourceBoneData>* skeleton;
    ResourceAnimationData* animation = nullptr;
    std::vector<Mat4f> transforms;
    int transformSize = 0;
    unsigned int transformArraySize = 0, prevTransformArraySize = 0;
    float animationTime = 0.0f, speed = 1.0f;
};