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
	std::array<std::string, 2> header = {
		// vertex
		"layout(location = 0) in vec3 VertexPosition;\n" \
		"layout(location = 1) in vec2 VertexTexCoord;\n" \
		"layout(location = 2) in vec4 VertexColor;\n" \
		"uniform mat4 uProjection;\n" \
		"uniform mat4 uView;\n" \
		"uniform mat4 uModel;\n"\
		"out vec3 VertPosition;\n" \
		"out vec2 TexCoord;\n" \
		"out vec4 VertColor;\n",

		// fragment
		"in vec3 VertPosition;\n" \
		"in vec2 TexCoord;\n" \
		"in vec4 VertColor;\n" \
		"out vec4 FragColor;\n"
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
		"void main() {\n" \
		"VertPosition = VertexPosition;\n" \
		"TexCoord = VertexTexCoord;\n" \
		"VertColor = VertexColor;\n" \
		"vec4 vertpos = vec4(VertexPosition.xyz, 1.0);\n" \
		"gl_Position = position((uProjection * uView * uModel), vertpos);\n" \
		"}\n",

		// fragment
		"void main() {\n" \
		"FragColor = effect(VertColor);\n" \
		"}\n"
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
auto createShader(const std::string& arg1 = std::string(), const std::string& arg2 = std::string()) {
	auto vertex = detail::glsl.default[0], fragment = detail::glsl.default[1];
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
