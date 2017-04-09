#pragma once

#define NOMINMAX

// gl3w
#include <opengl/gl3w.h>

// glfw
#include <GLFW/glfw3.h>

typedef struct {
	typedef struct {
		int width;
		int height;
		int minwidth;
		int minheight;
		bool decorated : true;
		bool focused : true;
		bool floating : true;
		bool resizable : true;
		bool vsyn : true;
	} Window;

	Window window;
} Config;

typedef struct {
	void(*init)(Config* config) = nullptr;
	void(*load)() = nullptr;
	void(*update)() = nullptr;
	void(*draw)() = nullptr;
	void(*quit)() = nullptr;
	void(*keyPressed)(int, int) = nullptr;
	void(*keyReleased)(int, int) = nullptr;
	void(*mouseMotion)() = nullptr;
	void(*mousePressed)(int, int, int) = nullptr;
	void(*mouseReleased)(int, int, int) = nullptr;
} Application;

static int run(Application* app) {
	bool quit = false;
	auto window = glfwGetCurrentContext();

	if (app->load) { app->load(); }

	do {
		// poll for events
		glfwPollEvents();

		if (app->update) { app->update(); }
		if (app->draw) {app->draw(); }

		glfwSwapBuffers(window);

		quit &= (glfwWindowShouldClose(window) != GL_TRUE);
	} while(!quit);

	if (app->quit) { app->quit(); }

	// terminate glfw
	glfwTerminate();

	return 0;
}

static void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode) {
	Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
	switch (action) {
	case GLFW_PRESS:
		app->keyPressed(key, scancode);
		break;
	case GLFW_RELEASE:
		app->keyReleased(key, scancode);
		break;
	default:
		break;
	}
}

static void glfw_onMouse(GLFWwindow* window, int button, int action, int mods) {
	Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));

	double x, y;
	glfwGetCursorPos(window, &x, &y);

	switch (action) {
	case GLFW_PRESS:
		app->mousePressed((int)x, (int)y, button);
		break;
	case GLFW_RELEASE:
		app->mouseReleased((int)x, (int)y, button);
		break;
	default:
		break;
	}
}

static int boot(Application* app) {
	if (app == nullptr) {
		return 0;
	}

	// initialize glfw
	if (!glfwInit()) {
		printf("Failed to initialize GLFW\n");
		return 0;
	}

	// initialize modules by checking bisous::Config
	GLFWwindow* window = nullptr;

	// create window
	const auto createWindow = [&](const Config& config) -> bool {

		glfwWindowHint(GLFW_DECORATED, config.window.decorated);
		glfwWindowHint(GLFW_FOCUSED, config.window.focused);
		glfwWindowHint(GLFW_FLOATING, config.window.floating);
		glfwWindowHint(GLFW_RESIZABLE, config.window.resizable);

		// request OpenGL context
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		// create window
		window = glfwCreateWindow(
			config.window.width,
			config.window.height,
			"COMP 371 - Final Project",
			nullptr, nullptr
			);

		if (!window) {
			return false;
		}

		

		glfwSwapInterval(config.window.vsyn);
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, app);

		// initialize gl3w
		gl3wInit();

		// Enable depth test
		glEnable(GL_DEPTH_TEST);

		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);

		// just for now
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		return true;
	};

	Config config;

	// initialize application
	app->init(&config);

	if (!createWindow(config)) {
		printf("Failed to create Window\n");
		return 0;
	}

	// set glfw callbacks
	//glfwSetFramebufferSizeCallback(window, glfw_onResize);
	glfwSetKeyCallback(window, glfw_onKey);
	glfwSetMouseButtonCallback(window, glfw_onMouse);

	return run(app);
}