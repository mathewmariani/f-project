// C/C++
#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>

// includes
#include "matty/application.h"
#include "matty/platform.h"
#include "matty/shader.h"
#include "matty/mat4.h"
#include "matty/vec3.h"
#include "matty/mesh.h"
#include "utils.h"

namespace shaders {

std::unordered_map<std::string, std::shared_ptr<Shader>> shader;

void initialize() {
	Shader::ShaderSource std_shader{
		"#version 330 core\n" \
		"layout(location = 0) in vec3 in_Vertex;" \
		"layout(location = 1) in vec3 vertexColor;" \
		"uniform mat4 uf_Projection;" \
		"uniform mat4 uf_Transform;" \
		"uniform mat4 uf_Model;" \
		"out vec3  frag_Color;" \
		"void main() {" \
		"frag_Color = vertexColor;" \
		"gl_Position = (uf_Projection * uf_Transform * uf_Model) * vec4(in_Vertex.xyz, 1.0);" \
		"}",

		"#version 330 core\n" \
		"in vec3 frag_Color;" \
		"out vec3 color;" \
		"void main() {" \
		"color = frag_Color;" \
		"}"
	};

	shader["std"] = std::make_shared<Shader>(std_shader);
}

}

namespace game {

mat4f projection = mat4f::perspective(45.0f, ((float)600 / (float)533), 0.1f, 100.0f);
mat4f transform = mat4f::lookAt(vec3f(4.0f, 3.0f, -3.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f));

GLuint vao, vertexbuffer, colorbuffer;

// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
static const GLfloat g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

// One color for each vertex. They were generated randomly.
static const GLfloat g_color_buffer_data[] = {
	0.583f,  0.771f,  0.014f,
	0.609f,  0.115f,  0.436f,
	0.327f,  0.483f,  0.844f,
	0.822f,  0.569f,  0.201f,
	0.435f,  0.602f,  0.223f,
	0.310f,  0.747f,  0.185f,
	0.597f,  0.770f,  0.761f,
	0.559f,  0.436f,  0.730f,
	0.359f,  0.583f,  0.152f,
	0.483f,  0.596f,  0.789f,
	0.559f,  0.861f,  0.639f,
	0.195f,  0.548f,  0.859f,
	0.014f,  0.184f,  0.576f,
	0.771f,  0.328f,  0.970f,
	0.406f,  0.615f,  0.116f,
	0.676f,  0.977f,  0.133f,
	0.971f,  0.572f,  0.833f,
	0.140f,  0.616f,  0.489f,
	0.997f,  0.513f,  0.064f,
	0.945f,  0.719f,  0.592f,
	0.543f,  0.021f,  0.978f,
	0.279f,  0.317f,  0.505f,
	0.167f,  0.620f,  0.077f,
	0.347f,  0.857f,  0.137f,
	0.055f,  0.953f,  0.042f,
	0.714f,  0.505f,  0.345f,
	0.783f,  0.290f,  0.734f,
	0.722f,  0.645f,  0.174f,
	0.302f,  0.455f,  0.848f,
	0.225f,  0.587f,  0.040f,
	0.517f,  0.713f,  0.338f,
	0.053f,  0.959f,  0.120f,
	0.393f,  0.621f,  0.362f,
	0.673f,  0.211f,  0.457f,
	0.820f,  0.883f,  0.371f,
	0.982f,  0.099f,  0.879f
};

// vertex shader
std::string std_vertex_shader =
"#version 330 core\n" \
"layout(location = 0) in vec3 in_Vertex;" \
"layout(location = 1) in vec3 vertexColor;" \
"uniform mat4 uf_Projection;" \
"uniform mat4 uf_Transform;" \
"uniform mat4 uf_Model;" \
"out vec3  frag_Color;" \
"void main() {" \
"frag_Color = vertexColor;" \
"gl_Position = (uf_Projection * uf_Transform * uf_Model) * vec4(in_Vertex.xyz, 1.0);" \
"}";

// fragment shader
std::string std_fragment_shader =
"#version 330 core\n" \
"in vec3 frag_Color;" \
"out vec3 color;" \
"void main() {" \
"color = frag_Color;" \
"}";

// compile shader
Shader defaultShader;

void init(Config* config) {
	config->window.width = 600;
	config->window.height = 533;
	config->window.decorated = true;
	config->window.floating = false;
	config->window.focused = false;
	config->window.resizable = false;
	config->window.vsyn = true;

}

void load() {
	printf("Load\n");
	shaders::initialize();

	for (auto i = 0; i < 16; ++i) {
		printf("%.2f, ", *(&projection[0][0] + i));
	}
	printf("\n");
	for (auto i = 0; i < 16; ++i) {
		printf("%.2f, ", *(&transform[0][0] + i));
	}


	defaultShader = Shader({
		std_vertex_shader, std_fragment_shader
	});

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
}

void update() {
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto s = shaders::shader["std"];
	s->attach();
	s->setMatrix4("uf_Projection", projection);
	s->setMatrix4("uf_Transform", transform);
	s->setMatrix4("uf_Model", mat4f::identity());

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

}

void quit() {
	printf("Quit\n");
}

void mousePressed(int x, int y, int button) {
	printf("Mouse Pressed\n");
}

void mouseReleased(int x, int y, int button) {
	printf("Mouse Released\n");
}

void keyPressed(int key, int scancode) {
	printf("Key Pressed\n");
}

void keyReleased(int key, int scancode) {
	printf("Key Released\n");
}

}	// game


int main(int argc, const char** argv) {
	Application* app = new Application();

#if defined(DEBUG)
	printf("Debug Build\n");
#endif

	app->init = &game::init;
	app->load = &game::load;
	app->update = &game::update;
	app->draw = &game::draw;
	app->quit = &game::quit;
	app->mousePressed = &game::mousePressed;
	app->mouseReleased = &game::mouseReleased;
	app->keyPressed = &game::keyPressed;
	app->keyReleased = &game::keyReleased;

	boot(app);
	delete app;
	return 0;
}
