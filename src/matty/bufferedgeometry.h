#pragma once
#include "libraries/vec3.h"
#include "opengl/gl3w.h"

typedef struct {
	float x, y, z;
	float u, v;
	float r, g, b, a;
	vec3f v_normal;
public:
	vec3f v_pos() {
		return vec3f(x,y,z);
	}

} Vertex;

class BufferedGeometry {
public:
	virtual ~BufferedGeometry() = default;
	/*std::vector<GLfloat>*/
	std::vector<Vertex> computeNormalVector(const unsigned int* pIndices, unsigned int IndexCount, Vertex* pVertices, unsigned int VertexCount)
	{
		for (unsigned int i = 0; i < IndexCount; i += 3) {
			unsigned int Index0 = pIndices[i];
			unsigned int Index1 = pIndices[i + 1];
			unsigned int Index2 = pIndices[i + 2];
			vec3<float> v1 = pVertices[Index1].v_pos()- pVertices[Index0].v_pos();
			vec3<float> v2 = pVertices[Index2].v_pos() - pVertices[Index0].v_pos();
			vec3<float> Normal = vec3<float>::cross(v1, v2);
			Normal = vec3<float>::normalize(Normal);

			pVertices[Index0].v_normal += Normal;
			pVertices[Index1].v_normal += Normal;
			pVertices[Index2].v_normal += Normal;
		}
		//std::vector<GLfloat> normals;
		std::vector<Vertex> normals;
		for (unsigned int i = 0; i < VertexCount; i++) {
			pVertices[i].v_normal = vec3<float>::normalize(pVertices[i].v_normal);
			Vertex norm{ 0,0,0 };
			float x=0, y=0, z=0;
			norm.x = pVertices[i].v_normal[0];
			norm.y = pVertices[i].v_normal[1];
			norm.z = pVertices[i].v_normal[2];
			x = pVertices[i].v_normal[0];
			y = pVertices[i].v_normal[1];
			z = pVertices[i].v_normal[2];
			normals.push_back(norm);
		}

		return normals;
	}
};	// bufferedgeometry
