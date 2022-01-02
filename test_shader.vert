#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 mModel;
    mat4 mView;
    mat4 mProj;
} ubo;

layout(location = 0) in vec2 mesh;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 texCoords;
layout(location = 2) out vec3 vertexPosition;
layout(location = 3) out vec3 normalPosition;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main() {
	vec3 r;	
	float u = 2.0*mesh.x - 1.0;
	float v = 2.0*mesh.y - 1.0;

	r.x = u;
	r.y = 1.0;
	r.z = v;

	fragColor = vec3(0.0, 0.0, 1.0);
	texCoords = vec2(2.0*mesh.x, 2.0*mesh.y);
	vertexPosition = vec3(ubo.mView * ubo.mModel * vec4(normalize(r), 1.0));
	normalPosition = vec3(transpose(inverse(ubo.mView * ubo.mModel)) * vec4(normalize(r), 1.0));
	gl_Position = ubo.mProj * vec4(vertexPosition, 1.0);
}