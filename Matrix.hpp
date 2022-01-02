#pragma once
#include <vector>

#define PI 3.14159f

class Vector {
	//float v[4];
	void fill(float value);
public:
	float x;
	float y;
	float z;
	float w;
	Vector();
	Vector(float value);
	Vector(float _x, float _y, float _z, float _w);
	Vector(const Vector& v);
	float& operator[](int index);
};

class Matrix {
	Vector m[4];
public:
	Matrix();
	void identity();
	Vector& operator[](int index);
	Matrix& translate(const Vector& v);
	Matrix& scale(const Vector& v);
	Matrix& rotX(float phi);
	Matrix& rotY(float phi);
	Matrix& rotZ(float phi);
	Matrix& operator=(const Matrix& rhs);
	Matrix operator*(Matrix& rhs);
	void frustum(float r, float t, float n, float f);
	void print();
	Matrix position();
	Matrix orientation();
	Matrix invert();
};

std::vector<float> createMesh1D(int resolution);
std::vector<float> createMesh2D(int resolution);
std::vector<unsigned short> createLineIndices(int resolution);
std::vector<unsigned short> createMeshIndices(int resolution);
std::vector<unsigned short> createLinePatchIndieces(int resolution);