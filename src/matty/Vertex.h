#pragma once
#include <opengl/gl3w.h>
#include "matty/vec2.h"
#include "matty/vec3.h"

struct Vertex
{
	vec3<float> m_pos;
	vec2<float> m_tex;
	vec3<float> m_normal;

	Vertex() {}

	Vertex(vec3<float> pos, vec2<float> tex)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = vec3<float>(0.0f, 0.0f, 0.0f);
	}
};