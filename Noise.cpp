#include "Noise.hpp"
#include <math.h>
#include <iostream>

#define max(a,b) a>b?a:b

Noise::Noise() {
	seed = 0;
}

Noise::Noise(unsigned int _seed) {
	seed = _seed;
	dis = std::uniform_real_distribution<float>(-1.0f, 1.0f);
}

float Noise::getRandomNumber1D(float x, unsigned int seedOffset) {
	unsigned int* _seed = reinterpret_cast<unsigned int*>(&x);
	gen.seed(*_seed + seedOffset + seed);
	return dis(gen);
}

float Noise::getRandomNumber2D(Vector2 r, unsigned int seedOffset) {
	float f = dot(r, Vector2(12.9898f, 78.233f));
	unsigned int* _seed = reinterpret_cast<unsigned int*>(&f);
	gen.seed(*_seed + seedOffset + seed);
	return dis(gen);
}

float Noise::perlinNoise1D(float x) {
	float p = floor(x);
	x = x - p;

	float g_0 = getRandomNumber1D(p, seed_u);
	float g_1 = getRandomNumber1D(p + 1.0f, seed_u);
	//std::cout << "(" << g_0 << "," << g_1 << ")   ";
	float w_0 = g_0 * x;
	float w_1 = g_1 * (x - 1.0f);
	float s_x = 10.0f * pow(x, 3.0f) - 15.0f * pow(x, 4.0f) + 6.0f * pow(x, 5.0f);
	float w = (1.0f - s_x) * w_0 + s_x * w_1;
	return w;
}

float Noise::perlinNoise2D(Vector2 r) {
	Vector2 p = floor(r);
	r = r - p;

	Vector2 quadPoint0 = Vector2(0.0f, 0.0f);
	Vector2 quadPoint1 = Vector2(1.0f, 0.0f);
	Vector2 quadPoint2 = Vector2(0.0f, 1.0f);
	Vector2 quadPoint3 = Vector2(1.0f, 1.0f);

	Vector2 g_00 = Vector2(getRandomNumber2D(p + quadPoint0, seed_u), getRandomNumber2D(p + quadPoint0, seed_v));
	Vector2 g_10 = Vector2(getRandomNumber2D(p + quadPoint1, seed_u), getRandomNumber2D(p + quadPoint1, seed_v));
	Vector2 g_01 = Vector2(getRandomNumber2D(p + quadPoint2, seed_u), getRandomNumber2D(p + quadPoint2, seed_v));
	Vector2 g_11 = Vector2(getRandomNumber2D(p + quadPoint3, seed_u), getRandomNumber2D(p + quadPoint3, seed_v));

	float w_00 = dot(g_00, r - quadPoint0);
	float w_10 = dot(g_10, r - quadPoint1);
	float w_01 = dot(g_01, r - quadPoint2);
	float w_11 = dot(g_11, r - quadPoint3);

	float s_x = 10.0f * pow(r.x, 3.0f) - 15.0f * pow(r.x, 4.0f) + 6.0f * pow(r.x, 5.0f);
	float s_y = 10.0f * pow(r.y, 3.0f) - 15.0f * pow(r.y, 4.0f) + 6.0f * pow(r.y, 5.0f);

	float w_0 = (1.0f - s_x) * w_00 + s_x * w_10;
	float w_1 = (1.0f - s_x) * w_01 + s_x * w_11;

	float w = (1.0f - s_y) * w_0 + s_y * w_1;

	return w;
}

NoiseFilter::NoiseFilter(unsigned int _seed) {
	noise = Noise(_seed);
}

NoiseFilter::NoiseFilter(unsigned int _seed, NoiseSettings _settings) {
	noise = Noise(_seed);
	settings = _settings;
}

float NoiseFilter::evaluate(float x) {
	float noiseValue = 0.0f;
	float amplitude = 1.0f;
	float frequency = settings.baseRoughness;

	for (int i = 0; i < settings.numLayers; i++) {
		float v = noise.perlinNoise1D(frequency * x);
		noiseValue += (v + 1.0f) * 0.5f * amplitude; // Werte zwischen [0,1]
		frequency *= 2.0f;
		amplitude *= 0.5f;
	}
	noiseValue = max(0, noiseValue - settings.minValue);
	return noiseValue * settings.strength;
}

float NoiseFilter::evaluate(Vector2 r) {
	float noiseValue = 0.0f;
	float amplitude = 1.0f;
	float frequency = settings.baseRoughness;

	for (int i = 0; i < settings.numLayers; i++) {
		float v = noise.perlinNoise2D(frequency * r);
		noiseValue += (v + 1.0f) * 0.5f * amplitude; // Werte zwischen [0,1]
		frequency *= 2.0f;
		amplitude *= 0.5f;
	}
	noiseValue = max(0.0f, noiseValue - settings.minValue);
	return noiseValue * settings.strength;
}