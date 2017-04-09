#pragma once

#include "bufferedgeometry.h"

class PlaneBufferedGeometry : public BufferedGeometry {
public:
	PlaneBufferedGeometry() = default;
	PlaneBufferedGeometry(int, int, int, int);
	~PlaneBufferedGeometry();

	void render();

private: // private variables
	GLuint vao, vbo, ebo;
};	// planebufferedgeometry
