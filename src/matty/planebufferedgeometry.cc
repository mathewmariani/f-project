#include <iostream>
#include <vector>
#include "planebufferedgeometry.h"

#include "opengl/gl3w.h"

PlaneBufferedGeometry::PlaneBufferedGeometry(int w, int h, int ws, int hs) {

	auto width_half = (float)w / (float)2;
	auto height_half = (float)h / (float)2;

	auto segment_width = (float)w / (float)ws;
	auto segment_height = (float)h / (float)hs;

	// vertex array object
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);

	// vertex buffer object
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// generate vertices, normals and uvs
	for (auto i = 0; i <= hs; ++i) {
		auto y = i * segment_height - height_half;
		for (auto j = 0; j <= ws; ++j) {
			auto x = j * segment_width - width_half;
			vertices.push_back(x);
			vertices.push_back(0);
			vertices.push_back(-y);
		}
	}

	// buffer data
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// element buffer object
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// indices
	for (auto i = 0; i < hs; i++) {
		for (auto j = 0; j < ws; j++) {
			auto a = j + (ws + 1) * i;
			auto b = j + (ws + 1) * (i + 1);
			auto c = (j + 1) + (ws + 1) * (i + 1);
			auto d = (j + 1) + (ws + 1) * i;

			// faces
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(d);

			indices.push_back(b);
			indices.push_back(c);
			indices.push_back(d);
		}
	}

	// element buffer object
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

	// unbind vao
	//glBindVertexArray(0);
}

PlaneBufferedGeometry::~PlaneBufferedGeometry() {

}

void PlaneBufferedGeometry::render() {
	//glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
}
