#pragma once

// C/C++
#include <vector>

#include "bufferedgeometry.h"

class TerrainGeometry : public BufferedGeometry {
public:
	TerrainGeometry() = default;
	TerrainGeometry(int, int, int, int);
	~TerrainGeometry();

	void render();

private: // private variables
	GLuint vao, vbo, ebo, normalbuffer;

	// buffers
	std::vector<Vertex> vertices;
	//std::vector<float> vertices;
	std::vector<GLuint> indices;
	std::vector<Vertex>  normals;
};	// planebufferedgeometry
