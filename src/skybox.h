#pragma once

// C/C++
#include <vector>

#include "opengl/gl3w.h"
#include "libraries/soil/SOIL.h"
#include "matty/bufferedgeometry.h"

class Skybox : public BufferedGeometry {
public:
	Skybox();
	GLuint createSkyboxVao();
	GLuint createCubeMapTexture();

private:
	GLuint loadCubemap(std::vector<const GLchar*> faces);
	GLuint vao, vbo, ebo;

	// buffers
	std::vector<Vertex> vertices;
	//std::vector<float> vertices;
	//std::vector<float> uvs;
	std::vector<int> indices;
};
