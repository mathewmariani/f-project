
#include <iostream>
#include "matty/texturemanager.h"
#include "libraries/soil/SOIL.h"

void TextureManager::destroy(const std::string& name) {
	auto itr = cache.find(name);
	if (itr != cache.end()) {
		destroy(&itr->second);
		cache.erase(itr);
	}
}

void TextureManager::destroy(Texture* texture) {
	if (texture == nullptr) {
		return;
	}

	glDeleteTextures(1, &texture->handle);
}

Texture* TextureManager::get(const std::string& name) {
	auto itr = cache.find(name);
	return (itr != cache.end()) ? (&itr->second) : &(cache[name] = load(name));
}


Texture TextureManager::load(const std::string& name) {
	int w, h;
	auto image = SOIL_load_image(name.c_str(), &w, &h, 0, SOIL_LOAD_AUTO);

	if (!image) {
		std::cout << "Failed to load texture" << std::endl;
	}

	GLuint textureId;
	glGenTextures(1, &textureId);

	Texture texture;
	texture.handle = textureId;
	texture.width = w;
	texture.height = h;

	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image);

	glDisable (GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(image);

	return texture;
}

Texture TextureManager::getCubeMap(const std::vector<std::string>& names) {
	int w, h;
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	for (auto i = 0; i < names.size(); ++i) {
		auto image = SOIL_load_image(names[i].c_str(), &w, &h, 0, SOIL_LOAD_AUTO);

		if (!image) {
			std::cout << "Failed to load texture" << std::endl;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

		SOIL_free_image_data(image);
	}



	Texture texture;
	texture.handle = textureId;
	texture.width = w;
	texture.height = h;

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return texture;
}
