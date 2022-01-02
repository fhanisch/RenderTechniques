#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 mModel;
    mat4 mView;
    mat4 mProj;
} ubo;

out gl_PerVertex {
	vec4 gl_Position;
};

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inColor;
layout (location = 3) in vec2 inTexCoords;

layout(location = 0) out vec3 outFragColor;
layout(location = 1) out vec2 outTexCoords;
layout(location = 2) out vec3 vertexPosition;
layout(location = 3) out vec3 normalPosition;

void main() {
	outFragColor = inColor;
	outTexCoords = inTexCoords;
	vertexPosition = vec3(ubo.mView * ubo.mModel * vec4(inPosition, 1.0));
	normalPosition = vec3(transpose(inverse(ubo.mView * ubo.mModel)) * vec4(inNormal, 1.0));
	gl_Position = ubo.mProj * vec4(vertexPosition, 1.0);
}