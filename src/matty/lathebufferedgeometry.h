#pragma once

#include "bufferedgeometry.h"

class LatheBufferedGeometry : public BufferedGeometry {
public:
	LatheBufferedGeometry(const void*, int);
	~LatheBufferedGeometry();

	void render();

private: // private variables
	GLuint vao, vbo, ebo;
};	// planebufferedgeometry
