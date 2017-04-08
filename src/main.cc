// C/C++
#include <iostream>

// includes
#include "matty/application.h"
#include "matty/planebufferedgeometry.h"

namespace game {

PlaneBufferedGeometry* p;

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

	p = new PlaneBufferedGeometry(1, 1, 2, 2);
}

void update() {

}

void draw() {
	p->render();
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
