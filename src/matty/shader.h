#pragma once

// C/C++
#include <string>

// matty
#include "matty/mat4.h"
#include "matty/platform.h"

// gl3w
#include "opengl/gl3w.h"

class Shader {
public:
	typedef struct {
		std::string vertex;
		std::string fragment;
	} ShaderSource;

public:
	static Shader *current;

public:
	Shader() = default;
	Shader(const ShaderSource& source);
	~Shader();

	/**
	* Use this function to attach this shader to the current rendering state.
	*/
	void attach();

	/**
	* Use this function to detach this shader from the current rendering state.
	*/
	void detach();

	/**
	* Use this function to specify the value of a uniform variable for this shader.
	*/
	void setFloat(const std::string& name, float value);
	void setInteger(const std::string& name, int value);
	void setVector2f(const std::string& name, float x, float y);
	void setVector3f(const std::string& name, float x, float y, float z);
	void setMatrix4(const std::string& name, const mat4f& matrix);

private:
	void compile(const ShaderSource& source);

private:
	GLuint program;
};	// shader
