#include "GeometryData.hpp"

GeometryData::GeometryData() {
	vertices = nullptr;
	indices = nullptr;
	verticesSize = 0;
	indicesSize = 0;
}

GeometryData::~GeometryData() {

}

void GeometryData::createData(std::string _name, GeometryForm _geoForm, int resolution) {
	/* TODO: checken dass jede Form nur einmal vorkommt! */
	Descriptor descr;
	std::vector<float> verts;
	std::vector<uint16_t> inds;
	descr.name = _name;
	descr.geoForm = _geoForm;
	descr.resolution = 0;

	switch (_geoForm) {
	case GEO_PLANE:
		pushBackVertices((float*)verticesPlane.data(), (uint32_t)verticesPlane.size() * (sizeof(Vertex) / sizeof(float)), descr);
		pushBackIndices(indicesPlane.data(), (uint32_t)indicesPlane.size(), descr);
		break;
	case GEO_STAR:
		pushBackVertices((float*)verticesStar.data(), (uint32_t)verticesStar.size() * (sizeof(Vertex) / sizeof(float)), descr);
		pushBackIndices(indicesStar.data(), (uint32_t)indicesStar.size(), descr);
		break;
	case GEO_MESH_1D:
		descr.resolution = resolution;
		verts = createMesh1D(resolution);
		inds = createLineIndices(resolution);
		pushBackVertices(verts.data(), (uint32_t)verts.size(), descr);
		pushBackIndices(inds.data(), (uint32_t)inds.size(), descr);
		break;
	case GEO_MESH_2D:
		descr.resolution = resolution;
		verts = createMesh2D(resolution);
		inds = createMeshIndices(resolution);
		pushBackVertices(verts.data(), (uint32_t)verts.size(), descr);
		pushBackIndices(inds.data(), (uint32_t)inds.size(), descr);
		break;
	case GEO_CURVE_PATCHES:
		pushBackVertices(verticesCurvePatches.data(), (uint32_t)verticesCurvePatches.size(), descr);
		pushBackIndices(indicesCurvePatches.data(), (uint32_t)indicesCurvePatches.size(), descr);
		break;
	case GEO_PATCH_1D:
		descr.resolution = resolution;
		verts = createMesh1D(resolution);
		inds = createLinePatchIndieces(resolution);
		pushBackVertices(verts.data(), (uint32_t)verts.size(), descr);
		pushBackIndices(inds.data(), (uint32_t)inds.size(), descr);
		break;
	case GEO_CUBE:
		pushBackVertices((float*)verticesCube.data(), (uint32_t)verticesCube.size() * (sizeof(Vertex) / sizeof(float)), descr);
		pushBackIndices(indicesCube.data(), (uint32_t)indicesCube.size(), descr);
		break;
	case GEO_CUBE_SPHERE:
		pushBackVertices((float*)verticesCube.data(), (uint32_t)verticesCube.size() * (sizeof(Vertex) / sizeof(float)), descr);
		pushBackIndices(indicesCubeSpherePatches.data(), (uint32_t)indicesCubeSpherePatches.size(), descr);
		break;
	default:
		std::cout << "Unknown Geometry form!\n";
		exit(1);
	}
	descriptors.push_back(descr);
}

void GeometryData::pushBackVertices(const float* f, uint32_t size, Descriptor& descr) {
	float* tmp = vertices;
	vertices = new float[verticesSize + size];
	if (tmp) memcpy(vertices, tmp, verticesSize * sizeof(float));
	memcpy(&vertices[verticesSize], f, size * sizeof(float));
	delete[] tmp;
	descr.vertexOffset = verticesSize * sizeof(float);
	verticesSize += size;
}

void GeometryData::pushBackIndices(const uint16_t* s, uint32_t size, Descriptor& descr) {
	uint16_t* tmp = indices;
	indices = new uint16_t[indicesSize + size];
	if (tmp) memcpy(indices, tmp, indicesSize * sizeof(uint16_t));
	memcpy(&indices[indicesSize], s, size * sizeof(uint16_t));
	delete[] tmp;
	descr.firstIndex = indicesSize;
	descr.indexCount = size;
	indicesSize += size;
}

float* GeometryData::getVertices() {
	return vertices;
}

uint16_t* GeometryData::getIndices() {
	return indices;
}

uint32_t GeometryData::getVerticesSize() {
	return verticesSize;
}

uint32_t GeometryData::getIndicesSize() {
	return indicesSize;
}

Descriptor& GeometryData::operator[](GeometryForm form) {
	for (int i = 0; i < descriptors.size(); i++) {
		if (descriptors[i].geoForm == form) return descriptors[i];
	}
	std::cout << "Form does not exist!\n";
	exit(1);
}