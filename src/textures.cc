#include "textures.h"
#include "libraries/soil/SOIL.h"

void Textures::destroy(const std::string& name) {
	auto itr = cache.find(name);
	if (itr != cache.end()) {
		destroy(&itr->second);
		cache.erase(itr);
	}
}

void Textures::destroy(Texture* texture) {
	if (texture == nullptr) {
		return;
	}

	glDeleteTextures (1, &texture->handle);
}

Texture* Textures::get(const std::string& name) {
	auto itr = cache.find(name);
	return (itr != cache.end()) ? (&itr->second) : &(cache[name] = load(name));
}


Texture Textures::load(const std::string& name) {
	int w, h;
	auto image = SOIL_load_image(("assets/" + name).c_str(), &w, &h, 0, SOIL_LOAD_RGB);

	GLuint textureId;
	glGenTextures (1, &textureId);

	Texture texture;
	texture.handle = textureId;
	texture.width = w;
	texture.height = h;

	glEnable (GL_TEXTURE_2D);

	glBindTexture (GL_TEXTURE_2D, textureId);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image);

	glDisable (GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	return texture;
}
