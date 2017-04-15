/**
* biscuit
*
* Copyright (c) 2017 Mathew Mariani
*
* This library is free software; you can redistribute it and/or modify it
* under the terms of the MIT license. See LICENSE for details.
*/
#pragma once

// C/C++
#include <array>
#include <regex>
#include <stdexcept>
#include <string>
#include <utility>

namespace biscuit {

// shader stage
enum { GLSL_VERTEX, GLSL_FRAGMENT };

//std::string Shader::header{
//	"#version 330 core\n"
//	"#define TransformProjectionMatrix (uf_Projection * uf_Transform * uf_Model)\n"
//	"layout(location = 0) in vec3 VertexPosition;\n" \
//	"layout(location = 1) in vec2 VertexTexCoord;\n" \
//	"layout(location = 2) in vec4 VertexColor;\n" \
//	"uniform mat4 uf_Projection;\n" \
//	"uniform mat4 uf_Transform;\n" \
//	"uniform mat4 uf_Model;\n"\
//	"vec4 position(mat4 transform_proj, vec3 vertpos) {" \
//	"return transform_proj * vec4(vertpos.xyz, 1.0);" \
//	"}\n"
//};

// shader code table
struct GLSL {
	std::string version = \
		"#version 330 core\n";
	std::array<std::string, 2> header = {
		// vertex
		"layout(location = 0) in vec3 VertexPosition;" \
		"layout(location = 1) in vec2 VertexTexCoord;" \
		"layout(location = 2) in vec4 VertexColor;" \
		"uniform mat4 uProjection;" \
		"uniform mat4 uView;" \
		"uniform mat4 uModel;" \
		"out vec3 VertPosition;" \
		"out vec2 TexCoord;" \
		"out vec4 VertColor;",

		// fragment
		"in vec3 VertPosition;" \
		"in vec2 TexCoord;" \
		"in vec4 VertColor;" \
		"out vec4 FragColor;"
	};
	std::array<std::string, 2> default = {
		// vertex
		"vec4 position(mat4 transform_proj, vec4 vertpos) {" \
		"return transform_proj * vertpos;" \
		"}",

		// fragment
		"vec4 effect(vec4 vcolor) {" \
		"return vcolor;" \
		"}"
	};
	std::array<std::string, 2> main = {
		// vertex
		"void main() {" \
		"VertPosition = VertexPosition;" \
		"TexCoord = VertexTexCoord;" \
		"VertColor = VertexColor;" \
		"vec4 vertpos = vec4(VertexPosition.xyz, 1.0);" \
		"gl_Position = position((uProjection * uView * uModel), vertpos);" \
		"}",
		
		// fragment
		"void main() {" \
		"FragColor = effect(VertColor);" \
		"}"
	};
} glsl;

auto createShaderCode(int stage, const std::string& code) noexcept {
	return std::string{
		glsl.version + "\n" +
		glsl.header[stage] + "\n" +
		code + "\n" + "\n" +
		glsl.main[stage]
	};
}

auto isVertexShader(const std::string& src) noexcept {
	std::regex_constants::basic;
	std::regex r("^.*vec4\\s+position\\s*.*$");
	return std::regex_match(src, r);
}

auto isFragmentShader(const std::string& src) noexcept {
	std::regex r("^.*vec4\\s+effect\\s*.*$");
	return std::regex_match(src, r);
}

auto createShader(const std::string& vertex, const std::string& fragment) {
	if (!isVertexShader(vertex)) {
		throw std::logic_error("Could not parse vertex stage shader code.");
	}

	if (!isFragmentShader(fragment)) {
		throw std::logic_error("Could not parse fragment stage shader code.");
	}

	return std::make_pair(
		createShaderCode(GLSL_VERTEX, vertex),
		createShaderCode(GLSL_FRAGMENT, fragment)
		);
}

}	// biscuit
