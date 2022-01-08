#pragma once
#include "Matrix.hpp"
#include <random>

class Noise {
	unsigned int seed;
	std::mt19937 gen;
	std::uniform_real_distribution<float> dis;
	float getRandomNumber(unsigned int _seed);
public:
	Noise();
	Noise(unsigned int _seed);
	float perlinNoise1D(float x);
	float perlinNoise2D(Vector2 r);
	float perlinNoise3D(Vector3 r);
};

struct NoiseSettings {
	int numLayers = 1;
};

class NoiseFilter {
	Noise noise;
	NoiseSettings settings;
public:
	NoiseFilter(unsigned int _seed, NoiseSettings _settings);
	float evaluate(float x);
	float evaluate(Vector2 r);
	float evaliate(Vector3 r);
};