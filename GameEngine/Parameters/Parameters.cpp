#include "Parameters.h"
#include <mutex>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "../Logger/Logger.h"

static std::mutex mutex = std::mutex();
std::unordered_map<std::string, std::string> Parameters::stringParams;
std::unordered_map<std::string, int> Parameters::intParams;
std::unordered_map<std::string, float> Parameters::floatParams;
std::unordered_map<std::string, bool> Parameters::boolParams;
std::unordered_map<std::string, Vec2f> Parameters::vec2Params;
std::unordered_map<std::string, Vec3f> Parameters::vec3Params;
std::unordered_map<std::string, Vec4f> Parameters::vec4Params;
std::unordered_map<std::string, std::shared_ptr<Parameters::Registrable>> Parameters::customParams;
int Parameters::instances = 0;

struct DefaultRegistrable : Parameters::Registrable{
	std::string s;
	explicit DefaultRegistrable(std::string s) : s(std::move(s)){}

	~DefaultRegistrable() override = default;

	void init(const std::string& s) override{

	}
	std::string toString() const override{
		return s;
	}
};

Parameters::Parameters(std::string className) : className(std::move(className)) {
	std::lock_guard<std::mutex> lock(mutex);
	if (instances == 0) {
		readParams();
	}
	instances++;
}

Parameters::~Parameters() {
	instances--;
	if (instances == 0) {
		writeParams();
	}
}

//For single-line strings only!
std::string& Parameters::getString(const std::string &name, const std::string& defaultValue) const{
	auto p = stringParams.find(name);
	if (p == stringParams.end()) {
		stringParams[name] = defaultValue;
		return stringParams[name];
	} else {
		return p->second;
	}
}

int& Parameters::getInt(const std::string &name, int defaultValue) const{
	auto p = intParams.find(name);
	if (p == intParams.end()) {
		intParams[name] = defaultValue;
		return intParams[name];
	} else {
		return p->second;
	}
}

float& Parameters::getFloat(const std::string &name, float defaultValue) const{
	auto p = floatParams.find(name);
	if (p == floatParams.end()) {
		floatParams[name] = defaultValue;
		return floatParams[name];
	} else {
		return p->second;
	}
}

bool& Parameters::getBool(const std::string &name, bool defaultValue) const{
	auto p = boolParams.find(name);
	if (p == boolParams.end()) {
		boolParams[name] = defaultValue;
		return boolParams[name];
	} else {
		return p->second;
	}
}

Vec2f& Parameters::getVec2(const std::string &name, const Vec2f& defaultValue) const{
	auto p = vec2Params.find(name);
	if (p == vec2Params.end()) {
		vec2Params[name] = defaultValue;
		return vec2Params[name];
	} else {
		return p->second;
	}
}

Vec3f& Parameters::getVec3(const std::string &name, const Vec3f& defaultValue) const{
	auto p = vec3Params.find(name);
	if (p == vec3Params.end()) {
		vec3Params[name] = defaultValue;
		return vec3Params[name];
	} else {
		return p->second;
	}
}

Vec4f& Parameters::getVec4(const std::string &name, const Vec4f& defaultValue) const{
	auto p = vec4Params.find(name);
	if (p == vec4Params.end()) {
		vec4Params[name] = defaultValue;
		return vec4Params[name];
	} else {
		return p->second;
	}
}

void Parameters::putRegistrable(const std::string &name, const std::shared_ptr<Parameters::Registrable>& defaultValue){
	auto p = customParams.find(name);
	if (p != customParams.end()) {
		auto* value = dynamic_cast<DefaultRegistrable*>(p->second.get());
		if(value == nullptr){
            logWarn("Attempted to register an already existing param on ", className, " ", name, ", the already existing param will be kept.");
			return;
		}
		defaultValue->init(value->s);
	}
	customParams[name] = defaultValue;
}

Parameters::Registrable& Parameters::get(const std::string &name) const{
	auto p = customParams.find(name);
	if (p == customParams.end()) {
		customParams[name] = std::make_unique<DefaultRegistrable>("");
		return *customParams[name].get();
	} else {
		return *p->second.get();
	}
}

void Parameters::readParams() {
	std::ifstream f;
	f.open("Parameters.txt");

	if (!f.is_open()) {
        logError("\"Couldn't open Parameters.txt\"");
		return;
	}
    logInfo("Reading Parameters");

	std::string str;
	std::stringstream ss;

	while (std::getline(f, str)) {
		ss = std::stringstream(str);

		std::string name, type;
		ss >> name >> type;

		if(type == "string"){
			std::string s;
			std::getline(ss, s);
			if(s.size() > 1){
				stringParams[name] = s.substr(1, std::string::npos);
			}else{
				stringParams[name] = "";
			}
		}else if (type == "int") {
			int v;
			ss >> v;
			intParams[name] = v;
		} else if (type == "float") {
			float v;
			ss >> v;
			floatParams[name] = v;
		} else if (type == "bool") {
			float v;
			ss >> v;
			boolParams[name] = v;
		} else if (type == "vec2") {
			float x, y;
			ss >> x >> y;
			vec2Params[name] = Vec2f(x, y);
		} else if (type == "vec3") {
			float x, y, z;
			ss >> x >> y >> z;
			vec3Params[name] = Vec3f(x, y, z);
		} else if (type == "vec4") {
			float x, y, z, w;
			ss >> x >> y >> z >> w;
			vec4Params[name] = Vec4f(x, y, z, w);
		} else if (type == "custom") {
			std::string s;
			int n1, n2;
			do{
				std::string tmp;
				ss >> tmp;
				s += " " + tmp;

				n1 = (int)std::count(s.begin(), s.end(), '{');
				n2 = (int)std::count(s.begin(), s.end(), '}');

			}while(n1 != n2);
			s = s.substr(2, s.size()-4);
			customParams[name] = std::make_unique<DefaultRegistrable>(s);
		}
	}
}

void Parameters::writeParams() {
	std::ofstream f;
	f.open("Parameters.txt");

	if (!f.is_open()) {
        logError("Couldn't open Parameters.txt");
		return;
	}
    logInfo("Writing Parameters...");

	std::vector<std::string> v;

	for (const auto& [name, value] : stringParams) {
		v.push_back(name + " " + "string" + " " + value);
	}
	for (const auto& [name, value] : intParams) {
		v.push_back(name + " " + "int" + " " + std::to_string(value));
	}
	for (const auto& [name, value] : floatParams) {
		v.push_back(name + " " + "float" + " " + std::to_string(value));
	}
	for (const auto& [name, value] : boolParams) {
		v.push_back(name + " " + "bool" + " " + std::to_string(value));
	}
	for (const auto& [name, value] : vec2Params) {
		v.push_back(name + " " + "vec2" + " " + std::to_string(value.x) + " " + std::to_string(value.y));
	}
	for (const auto& [name, value] : vec3Params) {
		v.push_back(name + " " + "vec3" + " " + std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.z));
	}
	for (const auto& [name, value] : vec4Params) {
		v.push_back(name + " " + "vec4" + " " + std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.z) + " " + std::to_string(value.w));
	}
	for (const auto& [name, value] : customParams) {
		v.push_back(name + " custom " + "{" + value->toString() + "}");
	}
	std::sort(v.begin(), v.end());

	for (const std::string &s : v) {
		f << s << std::endl;
	}
    logInfo("Parameters written.");
}