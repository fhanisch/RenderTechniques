#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 mModel;
    mat4 mView;
    mat4 mProj;
} ubo;

layout (location = 0) in vec2 inMesh;
layout (location = 1) in vec3 inNormal;

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec3 outFragColor;
layout(location = 2) out vec2 outTexCoords;

void main() {
	outNormal = inNormal;
	outFragColor = vec3(0.0, 1.0, 0.0);
	outTexCoords = inMesh;
	gl_Position = vec4(2.0*inMesh.x - 1.0, 0.0, 2.0*inMesh.y - 1.0, 1.0);
}