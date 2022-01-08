#include "Noise.hpp"
#include <math.h>
#include <iostream>

Noise::Noise() {
	seed = 0;
}

Noise::Noise(unsigned int _seed) {
	seed = _seed;
	dis = std::uniform_real_distribution<float>(-1.0f, 1.0f);
}

float Noise::getRandomNumber(unsigned int _seed) {
	gen.seed(_seed + seed);
	return dis(gen);
}

float Noise::perlinNoise1D(float x) {
	float p = floor(x);
	x = x - p;

	float g_0 = getRandomNumber((unsigned int)p + 513);
	float g_1 = getRandomNumber((unsigned int)p + 1 + 513);
	//std::cout << "(" << g_0 << "," << g_1 << ")   ";
	float w_0 = g_0 * x;
	float w_1 = g_1 * (x - 1.0f);
	float s_x = 10.0f * pow(x, 3.0f) - 15.0f * pow(x, 4.0f) + 6.0f * pow(x, 5.0f);
	float w = (1.0f - s_x) * w_0 + s_x * w_1;
	return w;
}

NoiseFilter::NoiseFilter(unsigned int _seed, NoiseSettings _settings) {
	noise = Noise(_seed);
	settings = _settings;
}