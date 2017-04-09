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
#include "matty/planebufferedgeometry.h"

namespace shaders {

std::unordered_map<std::string, std::shared_ptr<Shader>> shader;

void initialize() {
	Shader::ShaderSource std_shader{
		"#version 330 core\n" \
		"layout(location = 0) in vec4 in_Vertex;" \
		"layout(location = 1) in vec4 vertexColor;" \
		"uniform mat4 uf_Projection;" \
		"uniform mat4 uf_Transform;" \
		"uniform mat4 uf_Model;" \
		"out vec4 var_Color;" \
		"void main() {" \
		"var_Color = vertexColor;" \
		"gl_Position = (uf_Projection * uf_Transform * uf_Model) * vec4(in_Vertex.xyz, 1.0);" \
		"}",

		"#version 330 core\n" \
		"in vec4 var_Color;" \
		"out vec4 frag_Color;" \
		"void main() {" \
		"frag_Color = vec4(1.0, 1.0, 0.0, 0.0);" \
		"}"
	};

	shader["std"] = std::make_shared<Shader>(std_shader);
}

}

namespace game {

mat4f projection = mat4f::perspective(45.0f, ((float)600 / (float)533), 0.1f, 100.0f);
mat4f transform = mat4f::lookAt(vec3f(4.0f, 3.0f, -3.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f));

void init(Config* config) {
	config->window.width = 600;
	config->window.height = 533;
	config->window.decorated = true;
	config->window.floating = false;
	config->window.focused = false;
	config->window.resizable = false;
	config->window.vsyn = true;
}

PlaneBufferedGeometry plane;

void load() {
	printf("Load\n");
	shaders::initialize();

	plane = PlaneBufferedGeometry(4, 4, 4, 4);
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

	plane.render();
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
