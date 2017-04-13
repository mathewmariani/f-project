#pragma once
#include "matty/vec3.h"
#include "opengl/gl3w.h"

class BufferedGeometry {
public:
	virtual ~BufferedGeometry() = default;
	/*std::vector<GLfloat> computeNormalVector(const unsigned int* pIndices, unsigned int IndexCount, Vertex* pVertices, unsigned int VertexCount)
	{
		for (unsigned int i = 0; i < IndexCount; i += 3) {
			unsigned int Index0 = pIndices[i];
			unsigned int Index1 = pIndices[i + 1];
			unsigned int Index2 = pIndices[i + 2];
			vec3<float> v1 = pVertices[Index1].m_pos - pVertices[Index0].m_pos;
			vec3<float> v2 = pVertices[Index2].m_pos - pVertices[Index0].m_pos;
			vec3<float> Normal = vec3<float>::cross(v1, v2);
			Normal = vec3<float>::normalize(Normal);

			pVertices[Index0].m_normal += Normal;
			pVertices[Index1].m_normal += Normal;
			pVertices[Index2].m_normal += Normal;
		}
		std::vector<GLfloat> normals;
		for (unsigned int i = 0; i < VertexCount; i++) {
			pVertices[i].m_normal = vec3<float>::normalize(pVertices[i].m_normal);
			normals.push_back(pVertices[i].m_normal.x);
			normals.push_back(pVertices[i].m_normal.y);
			normals.push_back(pVertices[i].m_normal.z);
		}

		return normals;
	}*/
};	// bufferedgeometry
