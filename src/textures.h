#pragma once

// C/C++
#include <string>
#include <unordered_map>

// gl3w
#include "opengl/gl3w.h"

typedef struct {
	GLuint handle;
	int width, height;
} Texture;

class Textures {
public:
	Textures() = default;
	~Textures() = default;

	void destroy(const std::string& name);
	void destroy(Texture* texture);

	Texture* get(const std::string& name);

private:
	Texture load(const std::string& name);
	std::unordered_map<std::string, Texture> cache;
};
