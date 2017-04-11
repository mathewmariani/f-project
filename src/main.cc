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
#include"matty/camera.h"

namespace shaders {

	std::unordered_map<std::string, std::shared_ptr<Shader>> shader;

	void initialize() {
		Shader::ShaderSource std_shader{
			"#version 330 core\n" \
			"#define SCALE 10.0\n" \
			"layout(location = 0) in vec3 in_Vertex;" \
			"uniform mat4 uf_Projection;" \
			"uniform mat4 uf_Transform;" \
			"uniform mat4 uf_Model;" \
			"uniform float uf_Time;" \
			"vec4 position(mat4 transform_proj, vec3 vertpos) {" \
			"return transform_proj * vec4(vertpos.xyz, 1.0);" \
			"}" \
			"float calculateSurface(float x, float z) {" \
			"float y = 0.0;" \
			"y += (sin(x * 1.0 / SCALE + uf_Time * 1.0) + sin(x * 2.3 / SCALE + uf_Time * 1.5) + sin(x * 3.3 / SCALE + uf_Time * 0.4)) / 3.0;" \
			"y += (sin(z * 0.2 / SCALE + uf_Time * 1.8) + sin(z * 1.8 / SCALE + uf_Time * 1.8) + sin(z * 2.8 / SCALE + uf_Time * 0.8)) / 3.0;" \
			"return y;" \
			"}" \
			"void main() {" \
			"vec3 pos = in_Vertex;" \
			"pos.y += 1.0 * calculateSurface(pos.x, pos.z);" \
			"pos.y -= 1.0 * calculateSurface(0.0, 0.0);" \
			"gl_Position = position((uf_Projection * uf_Transform * uf_Model), pos);" \
			"}",

			"#version 330 core\n" \
			"out vec4 frag_Color;" \
			"void main() {" \
			"frag_Color = vec4(0.20, 0.59, 0.85, 1.00);" \
			"}"
		};
		shader["std"] = std::make_shared<Shader>(std_shader);
		Shader lightingShader("shader.vert", "shader.frag");
		shader["light"] = std::make_shared<Shader>(lightingShader);
	}

}

namespace game {

	
	Config* gameConfig;

	// Camera
	Camera  camera(vec3<float>(0.0f, 0.0f, 3.0f));
	GLfloat lastX = 0.0f;
	GLfloat lastY = 0.0f;
	bool    keys[1024];

	// Light attributes
	vec3<float> lightPos(1.2f, 1.0f, 2.0f);

	// Deltatime
	GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
	GLfloat lastFrame = 0.0f;  	// Time of last frame

	/// Handle mouse input
	bool mouseButtonLeftDown = false;
	double mousePosY = 0;
	double mousePosY_down = 0;

	mat4f projection = mat4f::perspective(45.0f, ((float)600 / (float)533), 0.1f, 100.0f);
	mat4f transform = camera.GetViewMatrix();//mat4f::lookAt(vec3f(4.0f, 3.0f, -3.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f));

	//prototype
	void initGameSettings();
	void do_movement();

	void init(Config* config) {
		config->window.width = 600;
		config->window.height = 533;
		config->window.decorated = true;
		config->window.floating = false;
		config->window.focused = false;
		config->window.resizable = false;
		config->window.vsyn = true;
		gameConfig = config;
	}

	void initGameSettings() {
		GLfloat lastX = gameConfig->window.width / 2.0;
		GLfloat lastY = gameConfig->window.height / 2.0;
	}
	PlaneBufferedGeometry plane;

	void load() {
		printf("Load\n");
		shaders::initialize();

		plane = PlaneBufferedGeometry(100, 100, 100, 100);
	}

	void update() {
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		do_movement();
	}
	void do_movement()
	{
		// Camera controls
		if (keys[GLFW_KEY_W])
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (keys[GLFW_KEY_S])
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (keys[GLFW_KEY_A])
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (keys[GLFW_KEY_D])
			camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	void draw2() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		transform = camera.GetViewMatrix();
		auto s = shaders::shader["std"];
		s->attach();
		s->setMatrix4("uf_Projection", projection);
		s->setMatrix4("uf_Transform", transform);
		s->setMatrix4("uf_Model", mat4f::identity());
		s->setFloat("uf_Time", (float)glfwGetTime());
		plane.render();
	}

	void draw() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto s = shaders::shader["std"];
		s->attach();
		s->setMatrix4("uf_Projection", projection);
		s->setMatrix4("uf_Transform", transform);
		s->setMatrix4("uf_Model", mat4f::identity());
		s->setFloat("uf_Time", (float)glfwGetTime());
		plane.render();
	}

	void quit() {
		printf("Quit\n");
	}

	void mousePressed(int x, int y, int button) {
		printf("Mouse Pressed\n");
		mouseButtonLeftDown = true;
	}

	void mouseReleased(int x, int y, int button) {
		printf("Mouse Released\n");
		mouseButtonLeftDown = false;
	}

	void keyPressed(int key, int scancode) {
		printf("Key Pressed\n");
		keys[key] = true;
		std::cout << "key: " << key << std::endl;
		switch (key)
		{
			//the user can change the rendering mode i.e. points, lines, triangles based on keyboard input 
			case	GLFW_KEY_P: {
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			}break;
			case	GLFW_KEY_O: {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}break;
			case	GLFW_KEY_T: {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}break;
		}
	}
	void keyReleased(int key, int scancode) {
		printf("Key Released\n");
		keys[key] = false;
	}
	bool firstMouse = true;
	void cursor_callback(GLFWwindow* window, double xpos, double ypos)
	{
		//if (mouseButtonLeftDown) {
			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			GLfloat xoffset = xpos - lastX;
			GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

			lastX = xpos;
			lastY = ypos;

			camera.ProcessMouseMovement(xoffset, yoffset,false);
		//}
		
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
		app->draw = &game::draw2;//draw;
		app->quit = &game::quit;
		app->mousePressed = &game::mousePressed;
		app->mouseReleased = &game::mouseReleased;
		app->keyPressed = &game::keyPressed;
		app->keyReleased = &game::keyReleased;

		boot(app);
		delete app;
		return 0;
	}
