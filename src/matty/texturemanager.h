#pragma once

// C/C++
#include <string>
#include <unordered_map>
#include <vector>

// gl3w
#include "opengl/gl3w.h"

typedef struct {
	GLuint handle;
	int width, height;
} Texture;

class TextureManager {
public:
	TextureManager() = default;
	~TextureManager() = default;

	void destroy(const std::string& name);
	void destroy(Texture* texture);

	Texture* get(const std::string& name);
  Texture getCubeMap(const std::vector<std::string>& names);

private:
	Texture load(const std::string& name);

private:
	std::unordered_map<std::string, Texture> cache;
};
