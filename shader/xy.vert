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

void main() {
	fragColor = vec3(0.0, 1.0, 0.0);
	texCoords = mesh;
	gl_Position = ubo.mProj * ubo.mView * ubo.mModel * vec4(mesh.x, mesh.y, 0.0, 1.0);
}