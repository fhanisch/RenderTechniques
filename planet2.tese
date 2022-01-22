#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 mModel;
    mat4 mView;
    mat4 mProj;
} ubo;

layout (binding = 2) uniform UniformBufferObject2 {
	float radius;
} MyValue;

layout( quads ) in;

layout (location = 0) in vec3 inNormal[];
layout (location = 1) in vec3 inFragColor[];
layout (location = 2) in vec2 inTexCoords[];

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 texCoords;
layout(location = 2) out vec3 vertexPosition;
layout(location = 3) out vec3 normalPosition;
layout(location = 4) out float outNoiseValue;

const float SEED_U = 4289.0;
const float SEED_V = 987.0;
const float SEED_W = 173.0;

/* Zufallszahl zwischen [-1,+1] */
float random3d(vec3 pos, float seed) {
  return 2.0 * fract( sin( dot(pos, vec3(12.9898,78.233,189.39581)) + seed) * 43758.5453123 ) - 1.0;
}

float perlin_interp3(vec3 r) {
	vec3 p = floor(r);
	r = r - p;
	//r = fract(r); // fract() returns the fractional part of x. This is calculated as x - floor(x)
	// fract(r) = r - floor(r) --> identisch 

	vec3 cubePoint0 = vec3(0.0,0.0,0.0);
	vec3 cubePoint1 = vec3(1.0,0.0,0.0);
	vec3 cubePoint2 = vec3(0.0,1.0,0.0);
	vec3 cubePoint3 = vec3(1.0,1.0,0.0);
	vec3 cubePoint4 = vec3(0.0,0.0,1.0);
	vec3 cubePoint5 = vec3(1.0,0.0,1.0);
	vec3 cubePoint6 = vec3(0.0,1.0,1.0);
	vec3 cubePoint7 = vec3(1.0,1.0,1.0);
	
    vec3 g_000 = vec3(random3d(p + cubePoint0, SEED_U), random3d(p + cubePoint0, SEED_V), random3d(p + cubePoint0, SEED_W));
    vec3 g_100 = vec3(random3d(p + cubePoint1, SEED_U), random3d(p + cubePoint1, SEED_V), random3d(p + cubePoint1, SEED_W));
    vec3 g_010 = vec3(random3d(p + cubePoint2, SEED_U), random3d(p + cubePoint2, SEED_V), random3d(p + cubePoint2, SEED_W));
    vec3 g_110 = vec3(random3d(p + cubePoint3, SEED_U), random3d(p + cubePoint3, SEED_V), random3d(p + cubePoint3, SEED_W));
    vec3 g_001 = vec3(random3d(p + cubePoint4, SEED_U), random3d(p + cubePoint4, SEED_V), random3d(p + cubePoint4, SEED_W));
    vec3 g_101 = vec3(random3d(p + cubePoint5, SEED_U), random3d(p + cubePoint5, SEED_V), random3d(p + cubePoint5, SEED_W));
    vec3 g_011 = vec3(random3d(p + cubePoint6, SEED_U), random3d(p + cubePoint6, SEED_V), random3d(p + cubePoint6, SEED_W));
    vec3 g_111 = vec3(random3d(p + cubePoint7, SEED_U), random3d(p + cubePoint7, SEED_V), random3d(p + cubePoint7, SEED_W));

    float w_000 = dot(g_000, r - cubePoint0);
    float w_100 = dot(g_100, r - cubePoint1);
    float w_010 = dot(g_010, r - cubePoint2);
    float w_110 = dot(g_110, r - cubePoint3);
    float w_001 = dot(g_001, r - cubePoint4);
    float w_101 = dot(g_101, r - cubePoint5);
    float w_011 = dot(g_011, r - cubePoint6);
    float w_111 = dot(g_111, r - cubePoint7);

    // Überblendungsfunktion: s(x) = 10x^3-15x^4+6x^5
    float s_x = pow(r.x,3.0)*(r.x*(r.x*6.0-15.0)+10.0);
    float s_y = pow(r.y,3.0)*(r.y*(r.y*6.0-15.0)+10.0);
    float s_z = pow(r.z,3.0)*(r.z*(r.z*6.0-15.0)+10.0);

    float w_00 = (1.0-s_x)*w_000 + s_x*w_100;
    float w_10 = (1.0-s_x)*w_010 + s_x*w_110;
    float w_01 = (1.0-s_x)*w_001 + s_x*w_101;
    float w_11 = (1.0-s_x)*w_011 + s_x*w_111;

    float w_0 = (1.0-s_y)*w_00 + s_y*w_10;
    float w_1 = (1.0-s_y)*w_01 + s_y*w_11;

    return (1.0-s_z)*w_0 + s_z*w_1;
}

void main() {
	vec3 r;
	vec3 n;
	vec3 P[4];
	P[0]=gl_in[0].gl_Position.xyz;
	P[1]=gl_in[1].gl_Position.xyz;
    P[2]=gl_in[2].gl_Position.xyz;
    P[3]=gl_in[3].gl_Position.xyz;
	float u = P[0].x + gl_TessCoord.x*(P[1].x-P[0].x);
    float v = P[0].z + gl_TessCoord.y*(P[2].z-P[0].z);

	vec3 axisA = vec3(inNormal[0].y, inNormal[0].z, inNormal[0].x);
	vec3 axisB = cross(inNormal[0], axisA);

	float noiseValue = 0.0;
	float elevation;
	float frequency = 2.0;
    float amplitude = 1.0;
	float minValue = 0.9;
	float strength = 0.5;
	float weight = 0.65;
	float weightMultiplier = 0.8;
	vec3 outColor = vec3(0.0, 0.0, 0.0);
	
	r = normalize(inNormal[0] + u*axisA + v*axisB);
	n = r;
	//r *= MyValue.radius;

	for (int i = 0; i < 4; i++) {
		float value = perlin_interp3(frequency * r);
		noiseValue += (value + 1.0) * 0.5 * amplitude;
		frequency *= 2.0;
        amplitude *= 0.5;

		/* Alternative
		/*
		float value = 1.0 - abs(perlin_interp3(n_out, frequency * r));
		value *= value;
		value *= weight;
		//weight = min(max(v * weightMultiplier, 0.0), 1.0);
		noiseValue += value * amplitude;
		frequency *= 2.0;
        amplitude *= 0.5;
		*/
	}

	noiseValue = (noiseValue - minValue) * strength;
	elevation = max(0.0, noiseValue);

	outNoiseValue = noiseValue;

	r *= MyValue.radius * (1.0 + elevation);

	fragColor = vec4(outColor, 1.0);
	texCoords = vec2(5.0*u, 5.0*v);
	vertexPosition = vec3(ubo.mView * ubo.mModel * vec4(r, 1.0));
	normalPosition = vec3(transpose(inverse(ubo.mView * ubo.mModel)) * vec4(n, 1.0));
	gl_Position = ubo.mProj * vec4(vertexPosition, 1.0);
}