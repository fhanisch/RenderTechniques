#pragma once

class Vec {
	//float v[4];
	void fill(float value);
public:
	float x;
	float y;
	float z;
	float w;
	Vec();
	Vec(float value);
	Vec(float _x, float _y, float _z, float _w);
	Vec(const Vec& v);
	float& operator[](int index);
};

class Matrix {
	Vec m[4];
	void identity();
public:
	Matrix();
	Vec& operator[](int index);
	void translate(const Vec& v);
	void rotZ(float phi);
};