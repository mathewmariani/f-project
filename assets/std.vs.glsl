#define TransformProjectionMatrix (uf_Transform * uf_Projection * uf_Model)

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexTexCoord;
layout(location = 2) in vec3 VertexColor;

uniform mat4 uf_Projection;
uniform mat4 uf_Transform;
uniform mat4 uf_Model;

vec4 position(mat4 transform_proj, vec3 vertpos) {
	return transform_proj * vec4(vertpos.xyz, 1.0);
}
