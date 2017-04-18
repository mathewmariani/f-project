// bisous
#include "matty/shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader* Shader::current{ nullptr };

std::string Shader::header{
	"#version 330 core\n" \
	"#define TransformProjectionMatrix (uf_Projection * uf_Transform * uf_Model)\n" \
	"layout(location = 0) in vec3 VertexPosition;\n" \
	"layout(location = 1) in vec2 VertexTexCoord;\n" \
	"layout(location = 2) in vec4 VertexColor;\n" \
	"layout(location = 3) in vec3 VertexNormal;\n"
	"uniform mat4 uf_Projection;\n" \
	"uniform mat4 uf_Transform;\n" \
	"uniform mat4 uf_Model;\n"\
	"vec4 position(mat4 transform_proj, vec3 vertpos) {" \
	"return transform_proj * vec4(vertpos.xyz, 1.0);" \
	"}\n"
};

Shader::Shader(const ShaderSource& source) {
	compile(source);
}

Shader::~Shader() {
	if (current == this) {
		detach();
		glDeleteShader(program);
	}
}

void Shader::attach() {
	if (current != this) {
		glUseProgram(program);
		current = this;
	}
}

void Shader::detach() {
	if (current == this) {
		current = nullptr;
	}
	glUseProgram(0);
}

void Shader::setFloat(const std::string& name, float value) {
	glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setInteger(const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setVector2f(const std::string& name, float x, float y) {
	glUniform2f(glGetUniformLocation(program, name.c_str()), x, y);
}

void Shader::setVector3f(const std::string& name, float x, float y, float z) {
	glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void Shader::setMatrix4(const std::string& name, const mat4f& matrix) {
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, matrix);
}

void Shader::compile(const ShaderSource& source) {
	// vertex shader
	auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
	auto concat = (Shader::header + source.vertex);
	const GLchar* vert = source.vertex.c_str();
	glShaderSource(vertexShader, 1, (const GLchar**)&vert, NULL);
	glCompileShader(vertexShader);

//#if defined(DEBUG)
	GLint successVert = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVert);

	if (successVert == GL_FALSE) {
		printf("Vertex Shader failed to compile.\n");
	}
#//endif

	// fragment shader
	auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* frag = source.fragment.c_str();
	glShaderSource(fragmentShader, 1, (const GLchar**)&frag, NULL);
	glCompileShader(fragmentShader);

//#if defined(DEBUG)
	GLint successFrag = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFrag);
    
	if (successFrag == GL_FALSE) {
		printf("Fragment Shader failed to compile.\n");
        int maxLength;
        int length;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
        char* log = new char[maxLength];
        glGetShaderInfoLog(fragmentShader, maxLength,&length,log);
        std::cout << log << std::endl;
	}
//#endif

	// attach shaders
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	

	// linker
	glLinkProgram(program);

//#if defined(DEBUG)
	GLint successLink = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &successLink);

	if (successLink == GL_FALSE) {
		printf("Shaders failed to link.\n");
        int maxLength;
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        char* log = new char[maxLength];
        glGetProgramInfoLog(program, maxLength,&length,log);
        std::cout << log << std::endl;
	}
//#endif

	//Clean up excess shader references
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{

	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensures ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try
	{
		// Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		//ShaderSource = fragmentCode(vertexCode, fragmentCode);
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar * fShaderCode = fragmentCode.c_str();
	Shader::ShaderSource new_shader{
		vertexCode.c_str(),
		fragmentCode.c_str()
	};
	
	// 2. Compile shaders
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];
	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// Print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Shader Program
	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	// Print linking errors if any
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);

}

/*Shader::ShaderSource Shader::readFromFile(const GLchar * vertexPath, const GLchar * fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensures ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try
	{
		// Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		//ShaderSource = fragmentCode(vertexCode, fragmentCode);
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	return ShaderSource{ vertexCode, fragmentCode };
}*/
