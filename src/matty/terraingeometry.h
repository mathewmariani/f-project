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
	std::vector<Vertex> vertices;
	//std::vector<float> vertices;
	std::vector<int> indices;
};	// planebufferedgeometry
