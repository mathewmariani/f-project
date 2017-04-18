/**
* biscuit.h
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

namespace detail {

// shader code table
struct GLSL {
	std::string version = \
		"#version 330 core\n";
  std::array<std::string, 2> header{{
		// vertex
		"layout(location = 0) in vec3 VertexPosition;" \
		"layout(location = 1) in vec2 VertexTexCoord;" \
		"layout(location = 2) in vec4 VertexColor;" \
		"layout(location = 3) in vec3 VertexNormal;" \
		"uniform mat4 uProjection;" \
		"uniform mat4 uView;" \
		"uniform mat4 uModel;"\
		"out vec3 VertPosition;" \
		"out vec2 TexCoord;" \
		"out vec4 VertColor;"
		"out vec3 Normal;",

		// fragment
		"in vec3 VertPosition;" \
		"in vec2 TexCoord;" \
		"in vec4 VertColor;" \
		"in vec3 Normal;"\
		"out vec4 FragColor;"\
		}};
	std::array<std::string, 2> std{{
		// vertex
		"vec4 position(mat4 transform_proj, vec4 vertpos) {" \
		"return transform_proj * vertpos;" \
		"}",

		// fragment
		"vec4 effect(vec4 vcolor) {" \
		"return vcolor;" \
		"}"
		}};
  std::array<std::string, 2> main{{
		// vertex
		"void main() {" \
		"VertPosition = VertexPosition;" \
		"TexCoord = VertexTexCoord;" \
		"VertColor = VertexColor;" \
		"vec4 vertpos = vec4(VertexPosition.xyz, 1.0);" \
		"gl_Position = position((uProjection * uView * uModel), vertpos);" \
		"Normal = mat3(transpose(inverse(uModel))) * VertexNormal;"
		"}",

		// fragment
		"void main() {" \
		"FragColor = effect(VertColor);" \
		"}"
		}};
} glsl;

std::string createShaderCode(int stage, const std::string& code) noexcept {
	return std::string{
		glsl.version + "\n" +
		glsl.header[stage] + "\n" +
		code + "\n" + "\n" +
		glsl.main[stage]
	};
}

bool isVertexShader(const std::string& src) noexcept {
	std::regex r("^.*vec4\\s+position\\s*.*$");
	return std::regex_match(src, r);
}

bool isFragmentShader(const std::string& src) noexcept {
	std::regex r("^.*vec4\\s+effect\\s*.*$");
	return std::regex_match(src, r);
}

}	// detail

	// shader stage
enum { GLSL_VERTEX, GLSL_FRAGMENT };

// case 1:
//	both vert and frag are present
//	-> check both
//	-> create shader
// case 2:
//	vert, or frag is present
//	-> check which is present
//	-> use default for missing
//	-> create shader
// case 3:
//	neither is present
//	-> use default for both
//	-> create shader
std::pair<std::string, std::string> createShader(const std::string& arg1 = std::string(), const std::string& arg2 = std::string()) {
	auto vertex = detail::glsl.std[0], fragment = detail::glsl.std[1];
	if (!arg1.empty()) {
		if (detail::isVertexShader(arg1)) {
			vertex = arg1;
		} else if (detail::isFragmentShader(arg1)) {
			fragment = arg1;
		} else {
			throw std::logic_error("Could not parse fragment stage shader code.");
		}
	}

	if (!arg2.empty()) {
		if (detail::isVertexShader(arg2)) {
			vertex = arg2;
		} else if (detail::isFragmentShader(arg2)) {
			fragment = arg2;
		} else {
			throw std::logic_error("Could not parse fragment stage shader code.");
		}
	}

	return std::make_pair(
		detail::createShaderCode(GLSL_VERTEX, vertex),
		detail::createShaderCode(GLSL_FRAGMENT, fragment)
		);
}

}	// biscuit
