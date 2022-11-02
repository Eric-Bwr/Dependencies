#include "ModelAnimator.h"
#include "ModelLoader.h"

void ModelAnimator::playAnimation(ResourceAnimationData *animation, std::unordered_map<std::string, ResourceBoneData> *skeleton, float speed) {
    this->animation = animation;
    this->skeleton = skeleton;
    this->speed = speed;
    this->transformSize = (int)skeleton->size();
    this->transformArraySize = sizeof(Mat4f) * transformSize;
    transforms.clear();
    transforms.resize(transformSize);
    animationTime = 0.0;
    mapPositionIterator.clear();
    mapRotationIterator.clear();
    mapScalingIterator.clear();
}

void ModelAnimator::updateAnimation(double deltaFrameTime) {
    if (animation != nullptr) {
        if (animationTime >= animation->duration)
            animationTime = 0.0f;
        calculateBoneTransform(&animation->nodeHierachy, identityMatrix());
        animationTime += animation->ticksPerSecond * speed * (float)deltaFrameTime;
    }
}

void ModelAnimator::loadTransformations() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ModelLoader::ssbo);
    if(prevTransformArraySize < transformArraySize)
        glBufferData(GL_SHADER_STORAGE_BUFFER, transformArraySize, nullptr, GL_DYNAMIC_DRAW);
    prevTransformArraySize = transformSize;
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, transformArraySize, transforms.data());
}

void ModelAnimator::calculateBoneTransform(const ResourceAssimpNodeData *node, Mat4f parentTransform) {
    auto it = skeleton->find(node->name);
    auto nodeTransform = node->transformation;
    if (it != skeleton->end()) {
        const auto &bone = it->second;
        nodeTransform.resetIdentity();
        interpolateBone(nodeTransform, bone.name);
        transforms[bone.boneID] = parentTransform * nodeTransform * bone.offset;
    }
    auto globalTransform = parentTransform * nodeTransform;
    for (const auto &i : node->children)
        calculateBoneTransform(&i, globalTransform);
}

Vec3f ModelAnimator::interpolatePosition(const std::string &boneName) {
    auto& positions = animation->keyPositions[boneName];
    if(mapPositionIterator.find(boneName) == mapPositionIterator.end())
        mapPositionIterator[boneName] = positions.begin();
    auto& map = mapPositionIterator[boneName];
    auto cNextIterator = std::next(map, 1);
    if(animationTime > cNextIterator->first){
        map = cNextIterator;
        cNextIterator = std::next(map, 1);
    }else if(animationTime == 0.0f) {
        map = positions.begin();
        cNextIterator = std::next(map, 1);
    }
    float cKey = map->first;
    float nKey = cNextIterator->first;
    auto scaleFactor = (animationTime - cKey) / (nKey - cKey);
    return positions[cKey].lerp(positions[nKey], scaleFactor);
}

Quatf ModelAnimator::interpolateRotation(const std::string &boneName) {
    auto& rotations = animation->keyRotations[boneName];
    if(mapRotationIterator.find(boneName) == mapRotationIterator.end())
        mapRotationIterator[boneName] = rotations.begin();
    auto& map = mapRotationIterator[boneName];
    auto cNextIterator = std::next(map, 1);
    if(animationTime > cNextIterator->first){
        map = cNextIterator;
        cNextIterator = std::next(map, 1);
    }else if(animationTime == 0.0f) {
        map = rotations.begin();
        cNextIterator = std::next(map, 1);
    }
    float cKey = map->first;
    float nKey = cNextIterator->first;
    auto scaleFactor = (animationTime - cKey) / (nKey - cKey);
    return rotations[cKey].slerp(rotations[nKey], scaleFactor).norm();
}

Vec3f ModelAnimator::interpolateScaling(const std::string &boneName) {
    auto& scales = animation->keyScales[boneName];
    if(mapScalingIterator.find(boneName) == mapScalingIterator.end())
        mapScalingIterator[boneName] = scales.begin();
    auto& map = mapScalingIterator[boneName];
    auto cNextIterator = std::next(map, 1);
    if(animationTime > cNextIterator->first){
        map = cNextIterator;
        cNextIterator = std::next(map, 1);
    }else if(animationTime == 0.0f) {
        map = scales.begin();
        cNextIterator = std::next(map, 1);
    }
    float cKey = map->first;
    float nKey = cNextIterator->first;
    auto scaleFactor = (animationTime - cKey) / (nKey - cKey);
    return scales[cKey].lerp(scales[nKey], scaleFactor);
}

void ModelAnimator::interpolateBone(Mat4f& matrix, const std::string &boneName) {
    auto translation = interpolatePosition(boneName);
    auto rotation = interpolateRotation(boneName);
    auto scaling = interpolateScaling(boneName);
    matrix.m30 = translation.x;
    matrix.m31 = translation.y;
    matrix.m32 = translation.z;
    matrix = matrix * rotation.toMat4();
    matrix.scale(scaling);
}