#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 mModel;
    mat4 mView;
    mat4 mProj;
} ubo;

layout( quads ) in;

layout (location = 0) in vec3 inNormal[];
layout (location = 1) in vec3 inFragColor[];
layout (location = 2) in vec2 inTexCoords[];

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 texCoords;
layout(location = 2) out vec3 vertexPosition;
layout(location = 3) out vec3 normalPosition;

void main() {

	vec3 r;	
	float u = 2.0*gl_TessCoord.x - 1.0;
	float v = 2.0*gl_TessCoord.y - 1.0;

	vec3 axisA = vec3(inNormal[0].y, inNormal[0].z, inNormal[0].x);
	vec3 axisB = cross(inNormal[0], axisA);

	r = inNormal[0] + u*axisA + v*axisB;

	fragColor = inFragColor[0];
	texCoords = vec2(2.0*gl_TessCoord.x, 2.0*gl_TessCoord.y);
	vertexPosition = vec3(ubo.mView * ubo.mModel * vec4(normalize(r), 1.0));
	normalPosition = vec3(transpose(inverse(ubo.mView * ubo.mModel)) * vec4(normalize(r), 1.0));
	gl_Position = ubo.mProj * vec4(vertexPosition, 1.0);
}