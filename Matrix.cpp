#include "Matrix.hpp"
#include <math.h>

Vec::Vec() {
	fill(0);
}

Vec::Vec(float value) {
	fill(value);
}

Vec::Vec(float _x, float _y, float _z, float _w) {
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

Vec::Vec(const Vec& v) {
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

void Vec::fill(float value) {
	for (int i = 0; i < 4; i++) {
		//v[i] = 0;
		((float*)this)[i] = 0;
	}
}

float& Vec::operator[](int index) {
	//return v[index];
	return ((float*)this)[index];
}

Matrix::Matrix() {
	identity();
}

void Matrix::identity() {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			if (i == j)
				m[i][j] = 1.0f;
			else
				m[i][j] = 0.0f;
		}
}

Vec& Matrix::operator[](int index) {
	return m[index];
}

void Matrix::translate(const Vec& v) {
	identity();
	m[3][0] = v.x;
	m[3][1] = v.y;
	m[3][2] = v.z;
}

void Matrix::rotZ(float phi) {
	float s = sinf(phi);
	float c = cosf(phi);

	identity();
	m[0][0] = c; m[1][0] = -s;
	m[0][1] = s; m[1][1] = c;
}