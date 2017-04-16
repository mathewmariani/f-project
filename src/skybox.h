#pragma once

// C/C++
#include <vector>

#include "libraries/soil/SOIL.h"
#include "matty/bufferedgeometry.h"

class Skybox : public BufferedGeometry {
public:
	Skybox() = default;
	~Skybox();

	void initialize();
  void render();

private:	// private variables
	GLuint vao, vbo, ebo;

	// buffers
	std::vector<Vertex> vertices;
	//std::vector<float> vertices;
	//std::vector<float> uvs;
	std::vector<int> indices;
};
