#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 mModel;
    mat4 mView;
    mat4 mProj;
} ubo;
layout(binding = 1) uniform sampler2D texSampler;
layout (binding = 2) uniform UBO {
	vec4 myColor;
} col;

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec2 texCoords;
layout(location = 2) in vec3 vertexPosition;
layout(location = 3) in vec3 normalPosition;
layout(location = 4) in float noiseValue;

layout (location = 0) out vec4 outColor;

void main() {
	vec4 texColor = texture(texSampler, vec2(texCoords.x, texCoords.y));
	float deep = -min(noiseValue, 0.0);
	if (noiseValue <= 0.0) {
		outColor = vec4(0.0, 0.0, min(max(1.0 - deep/0.9/0.5*4.0, 0.1), 1.0), 1.0);
	}
	else {
		outColor = outColor = mix(inColor,texColor,0.5);
	}
}