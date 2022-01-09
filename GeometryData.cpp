#include "GeometryData.hpp"
#include "Noise.hpp"

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
	case GEO_CUBE_SPHERE_PATCHES:
		descr.resolution = resolution;
		createCubeSpherePatches((std::vector<Mesh>*)&verts, &inds, resolution);
		pushBackVertices(verts.data(), (uint32_t)verts.size(), descr);
		pushBackIndices(inds.data(), (uint32_t)inds.size(), descr);
		break;
	case GEO_CUBE_SPHERE_TERRAIN:
		descr.resolution = resolution;
		createCubeSphere((std::vector<Vertex>*)& verts, &inds, resolution);
		//recalculateNormals((std::vector<Vertex>*) & verts, &inds);
		pushBackVertices(verts.data(), (uint32_t)verts.size(), descr);
		pushBackIndices(inds.data(), (uint32_t)inds.size(), descr);
		break;
	case GEO_PERLIN_1D:
		descr.resolution = resolution;
		createPerlin1D((std::vector<Vector2>*)& verts, &inds, resolution);
		pushBackVertices(verts.data(), (uint32_t)verts.size(), descr);
		pushBackIndices(inds.data(), (uint32_t)inds.size(), descr);
		break;
	case GEO_PERLIN_CIRCLE:
		descr.resolution = resolution;
		createPerlinCircle((std::vector<Vector2>*) & verts, &inds, resolution);
		pushBackVertices(verts.data(), (uint32_t)verts.size(), descr);
		pushBackIndices(inds.data(), (uint32_t)inds.size(), descr);
		break;
	case GEO_TERRAIN_2D:
		descr.resolution = resolution;
		createTerrain2D((std::vector<Vertex>*)& verts, &inds, resolution);
		recalculateNormals((std::vector<Vertex>*)& verts, &inds);
		pushBackVertices(verts.data(), (uint32_t)verts.size(), descr);
		pushBackIndices(inds.data(), (uint32_t)inds.size(), descr);
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

void GeometryData::createCubeSpherePatches(std::vector<Mesh>* verts, std::vector<uint16_t>* inds, int resolution) {
	int vertSizeFace = resolution * resolution;
	int indSizeFace = 4 * (resolution - 1) * (resolution - 1);
	*verts = std::vector<Mesh>(6 * vertSizeFace);
	*inds = std::vector<uint16_t>(6 * indSizeFace);
	Vector3 n[] = { { 0.0f,  1.0f,  0.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f,  0.0f,  0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f,  0.0f,  1.0f }, { 0.0f, 0.0f, -1.0f } };

	for (int i = 0; i < 6; i++) {
		createTerrainFace(verts, inds, n[i], resolution, i*vertSizeFace, i*indSizeFace);
	}
}

void GeometryData::createCubeSphere(std::vector<Vertex>* verts, std::vector<uint16_t>* inds, int resolution) {
	int vertSizeFace = resolution * resolution;
	int indSizeFace = 3 * 2 * (resolution - 1) * (resolution - 1);
	*verts = std::vector<Vertex>(6 * vertSizeFace);
	*inds = std::vector<uint16_t>(6 * indSizeFace);
	Vector3 n[] = { { 0.0f,  1.0f,  0.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f,  0.0f,  0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f,  0.0f,  1.0f }, { 0.0f, 0.0f, -1.0f } };

	for (int i = 0; i < 6; i++) {
		createCubeSphereFace(verts, inds, n[i], resolution, i * vertSizeFace, i * indSizeFace);
	}
}

void GeometryData::createTerrainFace(std::vector<Mesh>* verts, std::vector<uint16_t>* inds, Vector3 normal, int resolution, int vertOffset, int indOffset) {
	int k = vertOffset;
	int patchIndex = indOffset;
	for (int i = 0; i < resolution; i++) {
		for (int j = 0; j < resolution; j++) {
			(*verts)[k].uv.x = (float)j / ((float)resolution - 1);
			(*verts)[k].uv.y = (float)i / ((float)resolution - 1);
			(*verts)[k].normal = normal;
			k++;

			if (i < (resolution - 1) && j < (resolution - 1)) {
				(*inds)[patchIndex] = i * resolution + j + vertOffset;
				(*inds)[patchIndex + 1] = i * resolution + j + 1 + vertOffset;
				(*inds)[patchIndex + 2] = (i + 1) * resolution + j + vertOffset;
				(*inds)[patchIndex + 3] = (i + 1) * resolution + j + 1 + vertOffset;
				patchIndex += 4;
			}
		}
	}
}

void GeometryData::createCubeSphereFace(std::vector<Vertex>* verts, std::vector<uint16_t>* inds, Vector3 normal, int resolution, int vertOffset, int indOffset) {
	int vertexIndex = vertOffset;
	int triangleIndex = indOffset;
	float u;
	float v;
	Vector3 axisA = Vector3(normal.y, normal.z, normal.x);
	Vector3 axisB = cross(normal, axisA);
	Vector3 r;
	for (int i = 0; i < resolution; i++) {
		for (int j = 0; j < resolution; j++) {
			u = 2.0f * ((float)j / ((float)resolution - 1)) - 1.0f;
			v = 2.0f * ((float)i / ((float)resolution - 1)) - 1.0f;
			r = normal + u * axisA + v * axisB;
			r = normalize(r);
			(*verts)[vertexIndex].pos = r;
			(*verts)[vertexIndex].normal = normal;
			(*verts)[vertexIndex].texCoords.x = (float)j / ((float)resolution - 1);
			(*verts)[vertexIndex].texCoords.y = (float)i / ((float)resolution - 1);
			(*verts)[vertexIndex].color = Vector3(1.0f);
			vertexIndex++;

			if (i < (resolution - 1) && j < (resolution - 1)) {
				(*inds)[triangleIndex] = i * resolution + j + vertOffset;
				(*inds)[triangleIndex + 1] = i * resolution + j + 1 + vertOffset;
				(*inds)[triangleIndex + 2] = (i + 1) * resolution + j + vertOffset;

				(*inds)[triangleIndex + 3] = i * resolution + j + 1 + vertOffset;
				(*inds)[triangleIndex + 4] = (i + 1) * resolution + j + 1 + vertOffset;
				(*inds)[triangleIndex + 5] = (i + 1) * resolution + j + vertOffset;

				triangleIndex += 6;
			}
		}
	}
}

void GeometryData::createPerlin1D(std::vector<Vector2>* verts, std::vector<uint16_t>* inds, int resolution) {
	NoiseSettings settings;
	settings.numLayers = 8;
	settings.minValue = 0.85f;
	NoiseFilter noiseFilter(66, settings);
	*verts = std::vector<Vector2>(resolution);
	*inds = std::vector<uint16_t>(resolution);
	float x;
	for (int i = 0; i < resolution; i++) {
		x = 2.0f * ((float)i / ((float)resolution - 1)) - 1.0f;
		(*verts)[i].x = x;
		(*verts)[i].y = -noiseFilter.evaluate(x);
		(*inds)[i] = i;
	}
}

void GeometryData::createPerlinCircle(std::vector<Vector2>* verts, std::vector<uint16_t>* inds, int resolution) {
	NoiseSettings settings;
	settings.numLayers = 4;
	settings.minValue = 0.9f;
	settings.strength = 2.0f;
	NoiseFilter noiseFilter(123, settings);
	*verts = std::vector<Vector2>(resolution);
	*inds = std::vector<uint16_t>(resolution);
	float u;
	float dr;
	for (int i = 0; i < resolution; i++) {
		u = (float)i / ((float)resolution - 1);
		(*verts)[i].x = cos(u * 2.0f * PI);
		(*verts)[i].y = -sin(u * 2.0f * PI);
		(*inds)[i] = i;
		dr = noiseFilter.evaluate((*verts)[i]);
		(*verts)[i] = (1 + dr) * (*verts)[i];
	}
}

void GeometryData::createTerrain2D(std::vector<Vertex>* verts, std::vector<uint16_t>* inds, int resolution) {
	int vertSize = resolution * resolution;
	int indSize = 3 * 2 * (resolution - 1) * (resolution - 1);
	*verts = std::vector<Vertex>(vertSize);
	*inds = std::vector<uint16_t>(indSize);
	NoiseSettings settings;
	settings.numLayers = 4;
	settings.minValue = 0.9f;
	//settings.strength = 2.0f;
	NoiseFilter noiseFilter(244, settings);
	int vertexIndex = 0;
	int triangleIndex = 0;
	float u;
	float v;
	Vector3 r;
	for (int i = 0; i < resolution; i++) {
		for (int j = 0; j < resolution; j++) {
			u = 2.0f * ((float)j / ((float)resolution - 1)) - 1.0f;
			v = 2.0f * ((float)i / ((float)resolution - 1)) - 1.0f;
			r.x = u;
			r.y = noiseFilter.evaluate(Vector2(u, v));
			r.z = v;
			(*verts)[vertexIndex].pos = r;
			(*verts)[vertexIndex].normal = Vector3(0.0f, 1.0f, 0.0f);
			(*verts)[vertexIndex].texCoords.x = 2.0f * (float)j / ((float)resolution - 1);
			(*verts)[vertexIndex].texCoords.y = 2.0f * (float)i / ((float)resolution - 1);
			(*verts)[vertexIndex].color = Vector3(1.0f);
			vertexIndex++;

			if (i < (resolution - 1) && j < (resolution - 1)) {
				(*inds)[triangleIndex] = i * resolution + j;
				(*inds)[triangleIndex + 1] = i * resolution + j + 1;
				(*inds)[triangleIndex + 2] = (i + 1) * resolution + j;

				(*inds)[triangleIndex + 3] = i * resolution + j + 1;
				(*inds)[triangleIndex + 4] = (i + 1) * resolution + j + 1;
				(*inds)[triangleIndex + 5] = (i + 1) * resolution + j;

				triangleIndex += 6;
			}
		}
	}
}

void GeometryData::recalculateNormals(std::vector<Vertex>* verts, std::vector<uint16_t>* inds) {
	Vector3 a;
	Vector3 b;
	Vector3 n;
	for (int i = 0; i < (*inds).size(); i += 3) {
		a = (*verts)[(*inds)[i + 1]].pos - (*verts)[(*inds)[i]].pos;
		b = (*verts)[(*inds)[i + 2]].pos - (*verts)[(*inds)[i]].pos;
		n = -normalize(cross(a, b));
		(*verts)[(*inds)[i]].normal = n;
		(*verts)[(*inds)[i + 1]].normal = n;
		(*verts)[(*inds)[i + 2]].normal = n;
	}
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