#pragma once

// C/C++
#include <string>
#include <unordered_map>

// gl3w
#include "opengl/gl3w.h"

class Textures {
public:
	Textures() = default;
	~Textures() = default;

	GLuint load(const std::string& name, const std::string& path);
	GLuint get(const std::string& name);

private:
	std::unordered_map<std::string, GLuint> cache;
};
