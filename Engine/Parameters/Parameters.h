#pragma once

#include <Math/Vector.h>
#include <string>

class Parameters {
public:
    static void Load(const char* path);
    template<typename T>
    static T Get(const std::string& key);
    static void Set(const std::string& key, const int& value);
    static void Set(const std::string& key, const float& value);
    static void Set(const std::string& key, const double& value);
    static void Set(const std::string& key, const std::string& value);
    static void Set(const std::string& key, const Vec2i& value);
    static void Set(const std::string& key, const Vec2f& value);
    static void Set(const std::string& key, const Vec2d& value);
    static void Set(const std::string& key, const Vec3i& value);
    static void Set(const std::string& key, const Vec3f& value);
    static void Set(const std::string& key, const Vec3d& value);
    static void Set(const std::string& key, const Vec4i& value);
    static void Set(const std::string& key, const Vec4f& value);
    static void Set(const std::string& key, const Vec4d& value);
    static void Flush();
};
