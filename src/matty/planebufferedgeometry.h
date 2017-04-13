#pragma once

#include "bufferedgeometry.h"

typedef struct {
	float x, y, z;
	float u, v;
} Vertex;

class PlaneBufferedGeometry : public BufferedGeometry {
public:
	PlaneBufferedGeometry() = default;
	PlaneBufferedGeometry(int, int, int, int);
	~PlaneBufferedGeometry();

	void render();

private: // private variables
	GLuint vao, vbo, ebo;

	// buffers
	std::vector<Vertex> vertices;
	//std::vector<float> vertices;
	//std::vector<float> uvs;
	std::vector<int> indices;
	
};	// planebufferedgeometry
