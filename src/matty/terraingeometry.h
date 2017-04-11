#pragma once

#include "bufferedgeometry.h"

class TerrainGeometry : public BufferedGeometry {
public:
	TerrainGeometry() : vao(0), vbo(0), ebo(0) {};
	TerrainGeometry(int, int, int, int);
	~TerrainGeometry();

	void render();

private: // private variables
	GLuint vao, vbo, ebo;

	// buffers
	std::vector<int> indices;
	std::vector<float> vertices;
};	// planebufferedgeometry
