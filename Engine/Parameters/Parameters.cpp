#include "Parameters.h"
#include <Logger/Logger.h>
#include <vector>
#include <regex>

static FILE* file;
static std::unordered_map<std::string, std::string> parameters;

void Parameters::Load(const char* path) {
    file = fopen(path, "r");
    if(file){
        char* line = nullptr;
        size_t len = 0;
        ssize_t read;
        while ((read = getline(&line, &len, file)) != -1) {
            std::string key;
            std::string value;
            bool isVal = false;
            for(int i = 0; i < read; i++){
                auto c = line[i];
                if(c == '\r' || c == '\n' || c == '\t')
                    continue;
                if(c == ':') {
                    isVal = true;
                    continue;
                }
                if(!isVal)
                    key += c;
                else
                    value += c;
            }
            parameters[key] = value;
        }
    }
    fclose(file);
    file = fopen(path, "w");
}

static std::vector<std::string> splitParameter(const std::string& key){
    std::regex re(" ");
    std::vector<std::string> splitContents;
    auto input = parameters[key];
    std::sregex_token_iterator iter(input.begin(), input.end(), re, -1);
    std::sregex_token_iterator end;
    while (iter != end)
        splitContents.push_back(*iter++);
    if(splitContents.empty()) {
        logError("Failed to extract parameter: ", key);
    }
    return splitContents;
}

static void setParameter(const std::string& key, const std::string& value) {
    parameters[key] = value;
    if (file) {
        fprintf(file, "%s:%s%s", key.c_str(), value.c_str(), "\n");
        fflush(file);
    }
}

template<>
int Parameters::Get<int>(const std::string &key) {
    return std::stoi(parameters[key]);
}

template<>
float Parameters::Get<float>(const std::string &key) {
    return std::stof(parameters[key]);
}

template<>
double Parameters::Get<double>(const std::string &key) {
    return std::stod(parameters[key]);
}

template<>
std::string Parameters::Get<std::string>(const std::string &key) {
    return parameters[key];
}

template<>
Vec2i Parameters::Get<Vec2i>(const std::string &key) {
    auto split = splitParameter(key);
    return {std::stoi(split[0]), std::stoi(split[1])};
}

template<>
Vec2f Parameters::Get<Vec2f>(const std::string &key) {
    auto split = splitParameter(key);
    return {std::stof(split[0]), std::stof(split[1])};
}

template<>
Vec2d Parameters::Get<Vec2d>(const std::string &key) {
    auto split = splitParameter(key);
    return {std::stod(split[0]), std::stod(split[1])};
}

template<>
Vec3i Parameters::Get<Vec3i>(const std::string &key) {
    auto split = splitParameter(key);
    return {std::stoi(split[0]), std::stoi(split[1]), std::stoi(split[2])};
}

template<>
Vec3f Parameters::Get<Vec3f>(const std::string &key) {
    auto split = splitParameter(key);
    return {std::stof(split[0]), std::stof(split[1]), std::stof(split[2])};
}

template<>
Vec3d Parameters::Get<Vec3d>(const std::string &key) {
    auto split = splitParameter(key);
    return {std::stod(split[0]), std::stod(split[1]), std::stod(split[2])};
}

template<>
Vec4i Parameters::Get<Vec4i>(const std::string &key) {
    auto split = splitParameter(key);
    return {std::stoi(split[0]), std::stoi(split[1]), std::stoi(split[2]), std::stoi(split[3])};
}

template<>
Vec4f Parameters::Get<Vec4f>(const std::string &key) {
    auto split = splitParameter(key);
    return {std::stof(split[0]), std::stof(split[1]), std::stof(split[2]), std::stof(split[3])};
}

template<>
Vec4d Parameters::Get<Vec4d>(const std::string &key) {
    auto split = splitParameter(key);
    return {std::stod(split[0]), std::stod(split[1]), std::stod(split[2]), std::stod(split[3])};
}

void Parameters::Set(const std::string& key, const int& value){
    setParameter(key, std::to_string(value));
}

void Parameters::Set(const std::string& key, const float& value){
    setParameter(key, std::to_string(value));
}

void Parameters::Set(const std::string& key, const double& value){
    setParameter(key, std::to_string(value));
}

void Parameters::Set(const std::string& key, const std::string& value){
    setParameter(key, value);
}

void Parameters::Set(const std::string& key, const Vec2i& value){
    setParameter(key, std::to_string(value.x) + " " + std::to_string(value.y));
}

void Parameters::Set(const std::string& key, const Vec2f& value){
    setParameter(key, std::to_string(value.x) + " " + std::to_string(value.y));
}

void Parameters::Set(const std::string& key, const Vec2d& value){
    setParameter(key, std::to_string(value.x) + " " + std::to_string(value.y));
}

void Parameters::Set(const std::string& key, const Vec3i& value){
    setParameter(key, std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.z));
}

void Parameters::Set(const std::string& key, const Vec3f& value){
    setParameter(key, std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.z));
}

void Parameters::Set(const std::string& key, const Vec3d& value){
    setParameter(key, std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.z));
}

void Parameters::Set(const std::string& key, const Vec4i& value){
    setParameter(key, std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.z) + " " + std::to_string(value.w));
}

void Parameters::Set(const std::string& key, const Vec4f& value){
    setParameter(key, std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.z) + " " + std::to_string(value.w));
}

void Parameters::Set(const std::string& key, const Vec4d& value){
    setParameter(key, std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.z) + " " + std::to_string(value.w));
}

Parameters::~Parameters() {
    if(file)
        fclose(file);
}
