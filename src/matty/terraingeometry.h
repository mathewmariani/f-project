#pragma once

#include "bufferedgeometry.h"

class TerrainGeometry : public BufferedGeometry {
public:
	TerrainGeometry() = default;
	TerrainGeometry(int, int, int, int);
	~TerrainGeometry();

	void render();

private: // private variables
	GLuint vao, vbo, ebo;

	// buffers
	std::vector<int> indices;
	std::vector<float> vertices;
};	// planebufferedgeometry
