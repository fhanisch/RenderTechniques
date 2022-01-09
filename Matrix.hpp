#pragma once
#include <vector>

#define PI 3.14159f

struct Vector2 {
	float x;
	float y;
	Vector2();
	Vector2(float f);
	Vector2(float x, float y);
	Vector2(float v[2]);
	float& operator[](int index);
};

Vector2 floor(Vector2 r);
float dot(const Vector2& lhs, const Vector2& rhs);
Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
Vector2 operator*(const Vector2& lhs, const float rhs);
Vector2 operator*(const float lhs, const Vector2& rhs);

struct Vector3 {
	float x;
	float y;
	float z;
	Vector3();
	Vector3(float f);
	Vector3(float x, float y, float z);
	Vector3(float v[3]);
	Vector3(const Vector3& v);
	void fill(float f);
	void print();
	float length();
	Vector3& operator=(const Vector3& rhs);
	float& operator[](int index);
	const float& operator[](int index) const;
};

Vector3 normalize(Vector3 in);
Vector3 operator+(const Vector3& lhs, const float rhs);
Vector3 operator+(const float lhs, const Vector3& rhs);
Vector3 operator+(const Vector3& lhs, const Vector3& rhs);
Vector3 operator-(const Vector3& lhs, const Vector3& rhs);
Vector3 operator*(const Vector3& lhs, const float rhs);
Vector3 operator*(const float lhs, const Vector3& rhs);
Vector3 operator-(const Vector3& rhs);
Vector3 cross(const Vector3& lhs, const Vector3& rhs);

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
	const float& operator[](int index) const;
};

Vector normalize(Vector in);
Vector operator+(const Vector& lhs, const float rhs);

class Matrix {
	Vector m[4];
public:
	Matrix();
	void identity();
	Vector& operator[](int index);
	const Vector& operator[](int index) const;
	Matrix& translate(const Vector& v);
	Matrix& scale(const Vector& v);
	Matrix& rotX(float phi);
	Matrix& rotY(float phi);
	Matrix& rotZ(float phi);
	Matrix& operator=(const Matrix& rhs);
	void frustum(float r, float t, float n, float f);
	void print();
	Matrix position();
	Matrix orientation();
	Matrix invert();
	Vector3 getTranslationVector();
};

Matrix operator*(const Matrix& lhs, const Matrix& rhs);

std::vector<float> createMesh1D(int resolution);
std::vector<float> createMesh2D(int resolution);
std::vector<unsigned short> createLineIndices(int resolution);
std::vector<unsigned short> createMeshIndices(int resolution);
std::vector<unsigned short> createLinePatchIndieces(int resolution);