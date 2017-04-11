// bisous
#include "matty/shader.h"

Shader* Shader::current{ nullptr };

std::string Shader::header{
	"#version 330 core\n"
	"#define TransformProjectionMatrix (uf_Projection * uf_Transform * uf_Model)\n"
	"layout(location = 0) in vec3 VertexPosition;\n" \
	"layout(location = 1) in vec3 VertexTexCoord;\n" \
	"layout(location = 2) in vec3 VertexColor;\n"
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
	const GLchar* vert = concat.c_str();
	glShaderSource(vertexShader, 1, (const GLchar**)&vert, NULL);
	glCompileShader(vertexShader);

#if defined(DEBUG)
	GLint successVert = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVert);

	if (successVert == GL_FALSE) {
		printf("Vertex Shader failed to compile.\n");
	}
#endif

	// fragment shader
	auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* frag = source.fragment.c_str();
	glShaderSource(fragmentShader, 1, (const GLchar**)&frag, NULL);
	glCompileShader(fragmentShader);

#if defined(DEBUG)
	GLint successFrag = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successFrag);

	if (successFrag == GL_FALSE) {
		printf("Fragment Shader failed to compile.\n");
	}
#endif

	// attach shaders
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	

	// linker
	glLinkProgram(program);

#if defined(DEBUG)
	GLint successLink = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &successLink);

	if (successLink == GL_FALSE) {
		printf("Shaders failed to link.\n");
	}
#endif

	//Clean up excess shader references
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
