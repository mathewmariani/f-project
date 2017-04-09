#include <iostream>
#include <vector>
#include "planebufferedgeometry.h"

#include "opengl/gl3w.h"

PlaneBufferedGeometry::PlaneBufferedGeometry(int w, int h, int ws, int hs) {

	// vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// vertex buffer object
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// buffer data
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// element buffer object
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// element buffer object
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

	// unbind vao
	glBindVertexArray(0);
}

PlaneBufferedGeometry::~PlaneBufferedGeometry() {

}

void PlaneBufferedGeometry::render() {
	glBindVertexArray(vao);
	glDrawElementsBaseVertex(GL_TRIANGLES, 9 * sizeof(int), GL_UNSIGNED_INT, 0, 0);
	glBindVertexArray(0);
}
