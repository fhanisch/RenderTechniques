#version 450
#extension GL_ARB_separate_shader_objects : enable

const float PI = 3.14159;

layout (binding = 0) uniform UniformBufferObject {
  mat4 mModel;
  mat4 mView;
  mat4 mProj;
} ubo;

layout (push_constant) uniform PushConstants {
	float seed_u;
	float seed_v;
} pushConsts;

layout (location = 0) in float u;

layout (location = 0) out vec3 fragColor;
layout (location = 1) out vec2 texCoords;

out gl_PerVertex {
    vec4 gl_Position;
};

/* Zufallszahl zwischen [-1,+1] */
float random2d(vec2 pos, float seed) {
  return 2.0 * fract( sin( dot(pos, vec2(12.9898,78.233)) + seed) * 43758.5453123 ) - 1.0;
}

float perlin_interp2(vec2 r) {
	float SEED_U = pushConsts.seed_u;
	float SEED_V = pushConsts.seed_v;
	vec2 p = floor(r);
	r = r - p; //r = fract(r);

	vec2 quadPoint0 = vec2(0.0, 0.0);
	vec2 quadPoint1 = vec2(1.0, 0.0);
	vec2 quadPoint2 = vec2(0.0, 1.0);
	vec2 quadPoint3 = vec2(1.0, 1.0);

	vec2 g_00 = vec2(random2d(p + quadPoint0, SEED_U), random2d(p + quadPoint0, SEED_V));
	vec2 g_10 = vec2(random2d(p + quadPoint1, SEED_U), random2d(p + quadPoint1, SEED_V));
	vec2 g_01 = vec2(random2d(p + quadPoint2, SEED_U), random2d(p + quadPoint2, SEED_V));
	vec2 g_11 = vec2(random2d(p + quadPoint3, SEED_U), random2d(p + quadPoint3, SEED_V));

	float w_00 = dot(g_00, r - quadPoint0);
	float w_10 = dot(g_10, r - quadPoint1);
	float w_01 = dot(g_01, r - quadPoint2);
	float w_11 = dot(g_11, r - quadPoint3);

	float s_x = 10.0*pow(r.x,3.0)-15.0*pow(r.x,4.0)+6.0*pow(r.x,5.0);
	float s_y = 10.0*pow(r.y,3.0)-15.0*pow(r.y,4.0)+6.0*pow(r.y,5.0);

	float w_0 = (1.0-s_x)*w_00 + s_x*w_10;
	float w_1 = (1.0-s_x)*w_01 + s_x*w_11;

	float w = (1.0-s_y)*w_0 + s_y*w_1;

	return w;
}

void main() {
    vec2 r;

    r.x = cos(u*2.0*PI);
    r.y = -sin(u*2.0*PI);

	float dr = 0.8*perlin_interp2(r) + 0.2*perlin_interp2(4.0*r);

	r=(1+dr)*r;

    fragColor = vec3(0.0, 1.0, 0.0);
    texCoords = vec2(0.0, 0.0);
    gl_Position = ubo.mProj * ubo.mView * ubo.mModel * vec4(r.x, r.y, 0.0, 1.0);
}