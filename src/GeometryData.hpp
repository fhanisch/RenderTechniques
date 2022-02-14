#pragma once

#include <iostream>
#include <vector>
#include "RenderObject.hpp"
#include "Noise.hpp"

enum GeometryForm {
	GEO_LINE = 1,
	GEO_PLANE = 2,
	GEO_STAR = 3,
	GEO_CUBE = 4,
	GEO_SPHERE = 5,
	GEO_MESH_1D = 6,
	GEO_MESH_2D = 7,
	GEO_CURVE_PATCHES = 8,
	GEO_PATCH_1D = 9,
	GEO_CUBE_SPHERE = 10,
	GEO_CUBE_SPHERE_PATCHES = 11,
	GEO_CUBE_SPHERE_TERRAIN = 12,
	GEO_PERLIN_1D = 13,
	GEO_PERLIN_CIRCLE = 14,
	GEO_TERRAIN_2D = 15
};

struct Descriptor {
	std::string name;
	GeometryForm geoForm;
	uint64_t vertexOffset;
	uint32_t indexCount;
	uint32_t firstIndex;
	int resolution;
};

struct ShapeSettings {
	float planetRadius = 1.0f;
	std::vector<NoiseLayer> noiseLayer;
};

class ShapeGenerator {
	ShapeSettings shapeSettings;
	std::vector<NoiseFilter*> noiseFilter;
public:
	ShapeGenerator(ShapeSettings _shapeSettings);
	~ShapeGenerator();
	Vector3 calcualtePointOnUnitSphere(Vector3 r);
};

class GeometryData {
private:
	float* vertices;
	uint16_t* indices;
	uint32_t verticesSize;
	uint32_t indicesSize;
	std::vector<Descriptor> descriptors;
	void pushBackVertices(const float* f, uint32_t size, Descriptor& descr);
	void pushBackIndices(const uint16_t* s, uint32_t size, Descriptor& descr);
	void createCubeSpherePatches(std::vector<Mesh>* verts, std::vector<uint16_t>* inds, int resolution);
	void createCubeSphere(std::vector<Vertex>* verts, std::vector<uint16_t>* inds, int resolution);
	void createTerrainFace(std::vector<Mesh>* verts, std::vector<uint16_t>* inds, Vector3 normal, int resolution, int vertOffset, int indOffset);	
	void createCubeSphereFace(std::vector<Vertex>* verts, std::vector<uint16_t>* inds, Vector3 normal, int resolution, int vertOffset, int indOffset);
	void createPerlin1D(std::vector<Vector2>* verts, std::vector<uint16_t>* inds, int resolution);
	void createPerlinCircle(std::vector<Vector2>* verts, std::vector<uint16_t>* inds, int resolution);
	void createTerrain2D(std::vector<Vertex>* verts, std::vector<uint16_t>* inds, int resolution);
	void recalculateNormals(std::vector<Vertex>* verts, std::vector<uint16_t>* inds);
public:
	GeometryData();
	~GeometryData();
	void createData(std::string _name, GeometryForm _geoForm, int resolution = 0);
	float* getVertices();
	uint16_t* getIndices();
	uint32_t getVerticesSize();
	uint32_t getIndicesSize();
	Descriptor& operator[](GeometryForm form);
};

const std::vector<Vertex> verticesPlane = {
	{ {  -1.0f,   1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
	{ {   1.0f,   1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
	{ {   1.0f,  -1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
	{ {  -1.0f,  -1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } }
};
const std::vector<Vertex> verticesStar = {
	{ {   0.0f,  -1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
	{ {  0.25f, -0.25f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
	{ { -0.25f, -0.25f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
	{ {   1.0f,   0.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
	{ {  0.25f,  0.25f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
	{ {   0.0f,   1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
	{ {  -0.25,  0.25f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
	{ {  -1.0f,   0.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } }
};
const std::vector<float> verticesCurvePatches = { 0.0f, 1.0f, 2.0f };
const std::vector<Vertex> verticesCube = {
	{ { -1.0f,  1.0f,  1.0f }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
	{ {  1.0f,  1.0f,  1.0f }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
	{ {  1.0f, -1.0f,  1.0f }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
	{ { -1.0f, -1.0f,  1.0f }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
										 
	{ { -1.0f,  1.0f, -1.0f }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
	{ {  1.0f,  1.0f, -1.0f }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
	{ {  1.0f, -1.0f, -1.0f }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
	{ { -1.0f, -1.0f, -1.0f }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
										 
	{ {  1.0f,  1.0f, -1.0f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
	{ {  1.0f,  1.0f,  1.0f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
	{ {  1.0f, -1.0f,  1.0f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
	{ {  1.0f, -1.0f, -1.0f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
										 
	{ { -1.0f,  1.0f, -1.0f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
	{ { -1.0f,  1.0f,  1.0f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
	{ { -1.0f, -1.0f,  1.0f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
	{ { -1.0f, -1.0f, -1.0f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
										 
	{ { -1.0f,  1.0f,  1.0f }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
	{ {  1.0f,  1.0f,  1.0f }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
	{ {  1.0f,  1.0f, -1.0f }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
	{ { -1.0f,  1.0f, -1.0f }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },

	{ { -1.0f, -1.0f,  1.0f }, {  0.0f, -1.0f,  0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
	{ {  1.0f, -1.0f,  1.0f }, {  0.0f, -1.0f,  0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
	{ {  1.0f, -1.0f, -1.0f }, {  0.0f, -1.0f,  0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
	{ { -1.0f, -1.0f, -1.0f }, {  0.0f, -1.0f,  0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } }
};

const std::vector<uint16_t> indicesPlane = { 0, 1, 2, 2, 3, 0 };
const std::vector<uint16_t> indicesStar = { 0, 1, 2, 1, 3, 4, 4, 5, 6, 6, 7, 2, 2, 1, 4, 4, 6, 2 };
const std::vector<uint16_t> indicesCurvePatches = { 0,1, 1,2 };
const std::vector<uint16_t> indicesCube = { 0, 1, 2, 2, 3, 0,   4, 5, 6, 6, 7, 4,   8, 9, 10, 10, 11, 8,   12, 13, 14, 14, 15, 12,   16, 17, 18, 18, 19, 16,   20, 21, 22, 22, 23, 20 };
const std::vector<uint16_t> indicesCubeSpherePatches = { 0, 1, 2, 3,   4, 5, 6, 7,   8, 9, 10, 11,   12, 13, 14, 15,   16, 17, 18, 19,   20, 21, 22, 23 };