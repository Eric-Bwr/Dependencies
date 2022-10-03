//Author: Briac
#pragma once

#include <string>
#include <sstream>
#include <unordered_map>
#include <memory>
#include "Math/Vector.h"

class Parameters {
public:
	explicit Parameters(std::string className);
	~Parameters();

	Parameters extend(const std::string& className){
		return Parameters(this->className + "#" + className);
	}

	std::string& getString(const std::string &name, const std::string& defaultValue = "") const;
	int& getInt(const std::string &name, int defaultValue = 0) const;
	float& getFloat(const std::string &name, float defaultValue = 0) const;
	bool& getBool(const std::string &name, bool defaultValue = 0) const;
	Vec2f& getVec2(const std::string &name, const Vec2f& defaultValue = Vec2f(0)) const;
	Vec3f& getVec3(const std::string &name, const Vec3f& defaultValue = Vec3f(0)) const;
	Vec4f& getVec4(const std::string &name, const Vec4f& defaultValue = Vec4f(0)) const;

	struct Registrable{
		Registrable() = default;
		virtual void init(const std::string& s) = 0;
		virtual std::string toString() const = 0;
		virtual ~Registrable() = default;

		void read(std::stringstream& ss, bool& v) const{
			ss >> v;
		}
		void read(std::stringstream& ss, int& v) const{
			ss >> v;
		}
		void read(std::stringstream& ss, float& v) const{
			ss >> v;
		}
		void read(std::stringstream& ss, Vec3f& v) const{
			ss >> v.x >> v.y >> v.z;
		}
		void read(std::stringstream& ss, Vec4f& v) const{
			ss >> v.x >> v.y >> v.z >> v.w;
		}

		void write(std::stringstream& ss, bool v) const{
			ss <<v <<" ";
		}
		void write(std::stringstream& ss, int v) const{
			ss <<v <<" ";
		}
		void write(std::stringstream& ss, float v) const{
			ss <<v <<" ";
		}
		void write(std::stringstream& ss, Vec3f v) const{
			ss <<v.x <<" " <<v.y <<" " <<v.z <<" ";
		}
		void write(std::stringstream& ss, Vec4f v) const{
			ss <<v.x <<" " <<v.y <<" " <<v.z <<" " <<v.w <<" ";
		}
	};

	void putRegistrable(const std::string &name, const std::shared_ptr<Parameters::Registrable>& defaultValue);
	Registrable& get(const std::string & name) const;
private:
    static std::unordered_map<std::string, std::string> stringParams;
    static std::unordered_map<std::string, int> intParams;
    static std::unordered_map<std::string, float> floatParams;
    static std::unordered_map<std::string, bool> boolParams;
    static std::unordered_map<std::string, Vec2f> vec2Params;
    static std::unordered_map<std::string, Vec3f> vec3Params;
    static std::unordered_map<std::string, Vec4f> vec4Params;
    static std::unordered_map<std::string, std::shared_ptr<Parameters::Registrable>> customParams;

	static int instances;

	std::string className;
    static void readParams();
    static void writeParams();
};