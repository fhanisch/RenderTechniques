#pragma once
#include "Matrix.hpp"
#include <random>

class Noise {
	unsigned int seed;
	unsigned int seed_u = 5813;
	unsigned int seed_v = 1207;
	unsigned int seed_w = 8356;
	std::mt19937 gen;
	std::uniform_real_distribution<float> dis;
	float getRandomNumber1D(float x, unsigned int seedOffset);
	float getRandomNumber2D(Vector2 r, unsigned int seedOffset);
	float getRandomNumber3D(Vector3 r, unsigned int seedOffset);
public:
	Noise();
	Noise(unsigned int _seed);
	float perlinNoise1D(float x);
	float perlinNoise2D(Vector2 r);
	float perlinNoise3D(Vector3 r);
};

enum FilterType { SIMPLE, RIDGID};

struct NoiseSettings {
	FilterType filterType;
};

struct SimpleNoiseSettings : NoiseSettings {
	int numLayers = 1;
	float baseRoughness = 1.0f;
	float roughness = 2.0f;
	float persistence = 0.5f;
	float strength = 1.0f;
	float minValue = 0.0f;
};

struct RidgidNoiseSettings : SimpleNoiseSettings {
	float weightMultiplier = 0.8f;
};

struct NoiseLayer {
	bool enabled = true;
	bool useFirstLayerAsMask = true;
	NoiseSettings* noiseSettings;
	NoiseLayer(NoiseSettings* noiseSettings) : noiseSettings(noiseSettings) {}
};

class NoiseFilter {
protected:
	NoiseSettings* noiseSettings;
	Noise noise;
public:
	~NoiseFilter();
	virtual float evaluate(Vector3 r) { return 0; }
};

class SimpleNoiseFilter : public NoiseFilter {
	SimpleNoiseSettings settings;
public:
	SimpleNoiseFilter(unsigned int _seed);
	SimpleNoiseFilter(unsigned int _seed, SimpleNoiseSettings* _settings);
	~SimpleNoiseFilter();
	float evaluate(float x);
	float evaluate(Vector2 r);
	float evaluate(Vector3 r);
};

class RidgidNoiseFilter : public NoiseFilter {
	RidgidNoiseSettings settings;
public:
	RidgidNoiseFilter(unsigned int _seed);
	RidgidNoiseFilter(unsigned int _seed, RidgidNoiseSettings* _settings);
	float evaluate(Vector3 r);
};