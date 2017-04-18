#include <iostream>
#include <math.h>
#include <vector>
#include "terraingeometry.h"

#include "libraries/PerlinNoise.h"
#include "libraries/noise.h"
#include "opengl/gl3w.h"

TerrainGeometry::TerrainGeometry(int w, int h, int ws, int hs) {

	auto width_half = (float)w / (float)2;
	auto height_half = (float)h / (float)2;

	auto segment_width = (float)w / (float)ws;
	auto segment_height = (float)h / (float)hs;

	// vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// vertex buffer object
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// persistence, frequency, amplitude, octaves
	//PerlinNoise pn;
	//pn.Set(1.0, 1.0, 1.0, 1.0, 255);

	const siv::PerlinNoise perlin(12345);
	const double frequency = 2.50;
	const double fx = w / 8.0;
	const double fy = h / 8.0;

	// generate vertices, normals and uvs
	for (auto i = 0; i <= hs; ++i) {
		auto y = i * segment_height - height_half;
		for (auto j = 0; j <= ws; ++j) {

			double nx = j / ws - 0.5;
			double ny = i / hs - 0.5;
			auto e = perlin.octaveNoise0_1(i / fx, j / fy, 8);
			auto n = std::pow(e, 2.5);

			auto x = j * segment_width - width_half;
			//vertices.push_back(x);
			//vertices.push_back(n);
			//vertices.push_back(-y);

			vertices.push_back({
				x, 15 * (float)n, -y,
				((float)j / ws), (1.0f - ((float)i / hs)),
				0.0f, 0.0f, 0.0f, (float)e,
				vec3f(0,0,0)
			});
		}
	}

	// buffer data
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// bind VertexPosition aatribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

	// bind VertexTexCoord attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
	
	// bind VertexColor attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));

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

	// generate normals
	normals = computeNormalVector(indices.data(), indices.size(), vertices.data(), vertices.size());

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);

	// Binding normal attribute
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vertex), &normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	// unbind vao
	glBindVertexArray(0);
}

TerrainGeometry::~TerrainGeometry() {

}

void TerrainGeometry::render() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
