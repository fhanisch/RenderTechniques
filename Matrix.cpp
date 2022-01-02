#include "Matrix.hpp"
#include <math.h>
#include <string.h>
#include <iostream>
#include <iomanip>

Vector::Vector() {
	fill(0);
}

Vector::Vector(float value) {
	fill(value);
}

Vector::Vector(float _x, float _y, float _z, float _w) {
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

Vector::Vector(const Vector& v) {
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

void Vector::fill(float value) {
	for (int i = 0; i < 4; i++) {
		//v[i] = 0;
		((float*)this)[i] = value;
	}
}

float& Vector::operator[](int index) {
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

Vector& Matrix::operator[](int index) {
	return m[index];
}

Matrix& Matrix::translate(const Vector& v) {
	identity();
	m[3][0] = v.x;
	m[3][1] = v.y;
	m[3][2] = v.z;

	return *this;
}

Matrix& Matrix::scale(const Vector& v) {
	identity();
	m[0][0] = v.x;
	m[1][1] = v.y;
	m[2][2] = v.z;

	return *this;
}

Matrix& Matrix::rotX(float phi) {
	float s = sinf(phi);
	float c = cosf(phi);

	identity();
	m[1][1] = c; m[2][1] = -s;
	m[1][2] = s; m[2][2] =  c;

	return *this;
}

Matrix& Matrix::rotY(float phi) {
	float s = sinf(phi);
	float c = cosf(phi);

	identity();
	m[0][0] =  c; m[2][0] = s;
	m[0][2] = -s; m[2][2] = c;

	return *this;
}

Matrix& Matrix::rotZ(float phi) {
	float s = sinf(phi);
	float c = cosf(phi);

	identity();
	m[0][0] = c; m[1][0] = -s;
	m[0][1] = s; m[1][1] =  c;

	return *this;
}

Matrix& Matrix::operator=(const Matrix& rhs) {
	memcpy(this, &rhs, sizeof(Matrix));
	return *this;
}

Matrix Matrix::operator*(Matrix& rhs) {
	Matrix retVal;
	int i, r, c;
	for (c = 0; c < 4; c++)
		for (r = 0; r < 4; r++) {
			retVal[c][r] = 0.0f;
			for (i = 0; i < 4; i++) retVal[c][r] += m[i][r] * rhs[c][i];
		}
	return retVal;
}

void Matrix::frustum(float r, float t, float n, float f) {
	m[0][0] = n / r; m[1][0] = 0.0f;   m[2][0] = 0.0f;         m[3][0] = 0.0f;
	m[0][1] = 0.0f;  m[1][1] = -n / t; m[2][1] = 0.0f;         m[3][1] = 0.0f;
	m[0][2] = 0.0f;  m[1][2] = 0.0f;   m[2][2] = -f / (f - n); m[3][2] = -f * n / (f - n);
	m[0][3] = 0.0f;  m[1][3] = 0.0f;   m[2][3] = -1.0f;        m[3][3] = 0.0f;
}

void Matrix::print() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << std::setw(10) << m[j][i];
		}
		std::cout << "\n";
	}
}

Matrix Matrix::position() {
	Matrix retVal;

	retVal[3][0] = m[3][0];
	retVal[3][1] = m[3][1];
	retVal[3][2] = m[3][2];

	return retVal;
}

Matrix Matrix::orientation() {
	Matrix retVal;

	memcpy(&retVal, this, 12 * sizeof(float));

	return retVal;
}

Matrix Matrix::invert() {
	Matrix t;

	float s[6];
	float c[6];
	s[0] = m[0][0] * m[1][1] - m[1][0] * m[0][1];
	s[1] = m[0][0] * m[1][2] - m[1][0] * m[0][2];
	s[2] = m[0][0] * m[1][3] - m[1][0] * m[0][3];
	s[3] = m[0][1] * m[1][2] - m[1][1] * m[0][2];
	s[4] = m[0][1] * m[1][3] - m[1][1] * m[0][3];
	s[5] = m[0][2] * m[1][3] - m[1][2] * m[0][3];

	c[0] = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	c[1] = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	c[2] = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	c[3] = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	c[4] = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	c[5] = m[2][2] * m[3][3] - m[3][2] * m[2][3];

	/* Assumes it is invertible */
	float idet = 1.0f / (s[0] * c[5] - s[1] * c[4] + s[2] * c[3] + s[3] * c[2] - s[4] * c[1] + s[5] * c[0]);

	t[0][0] = ( m[1][1] * c[5] - m[1][2] * c[4] + m[1][3] * c[3]) * idet;
	t[0][1] = (-m[0][1] * c[5] + m[0][2] * c[4] - m[0][3] * c[3]) * idet;
	t[0][2] = ( m[3][1] * s[5] - m[3][2] * s[4] + m[3][3] * s[3]) * idet;
	t[0][3] = (-m[2][1] * s[5] + m[2][2] * s[4] - m[2][3] * s[3]) * idet;

	t[1][0] = (-m[1][0] * c[5] + m[1][2] * c[2] - m[1][3] * c[1]) * idet;
	t[1][1] = ( m[0][0] * c[5] - m[0][2] * c[2] + m[0][3] * c[1]) * idet;
	t[1][2] = (-m[3][0] * s[5] + m[3][2] * s[2] - m[3][3] * s[1]) * idet;
	t[1][3] = ( m[2][0] * s[5] - m[2][2] * s[2] + m[2][3] * s[1]) * idet;

	t[2][0] = ( m[1][0] * c[4] - m[1][1] * c[2] + m[1][3] * c[0]) * idet;
	t[2][1] = (-m[0][0] * c[4] + m[0][1] * c[2] - m[0][3] * c[0]) * idet;
	t[2][2] = ( m[3][0] * s[4] - m[3][1] * s[2] + m[3][3] * s[0]) * idet;
	t[2][3] = (-m[2][0] * s[4] + m[2][1] * s[2] - m[2][3] * s[0]) * idet;

	t[3][0] = (-m[1][0] * c[3] + m[1][1] * c[1] - m[1][2] * c[0]) * idet;
	t[3][1] = ( m[0][0] * c[3] - m[0][1] * c[1] + m[0][2] * c[0]) * idet;
	t[3][2] = (-m[3][0] * s[3] + m[3][1] * s[1] - m[3][2] * s[0]) * idet;
	t[3][3] = ( m[2][0] * s[3] - m[2][1] * s[1] + m[2][2] * s[0]) * idet;

	return t;
}

std::vector<float> createMesh1D(int resolution) {
	std::vector<float> v;
	float f;

	for (int i = 0; i < resolution; i++) {
		f = (float)i / (float)(resolution - 1);
		v.push_back(f);
	}

	return v;
}

std::vector<float> createMesh2D(int resolution) {
	std::vector<float> v;
	float f;

	for (int i = 0; i < resolution; i++)
		for (int j = 0; j < resolution; j++)
		{
			f = (float)j / ((float)resolution - 1);
			v.push_back(f);
			f = (float)i / ((float)resolution - 1);
			v.push_back(f);
		}

	return v;
}

std::vector<unsigned short> createLineIndices(int resolution) {
	std::vector<unsigned short> v;
	for (int i = 0; i < resolution; i++) {
		v.push_back(i);
	}
	return v;
}

std::vector<unsigned short> createMeshIndices(int resolution) {
	int len = 3 * 2 * (resolution - 1) * (resolution - 1);
	std::vector<unsigned short> v(len);

	for (int i = 0; i < (resolution - 1); i++)
		for (int j = 0; j < (resolution - 1); j++) {
			v[6 * (i * (resolution - 1) + j) + 0] = i * resolution + j;
			v[6 * (i * (resolution - 1) + j) + 1] = i * resolution + j + 1;
			v[6 * (i * (resolution - 1) + j) + 2] = (i + 1) * resolution + j;

			v[6 * (i * (resolution - 1) + j) + 3] = i * resolution + j + 1;
			v[6 * (i * (resolution - 1) + j) + 4] = (i + 1) * resolution + j;
			v[6 * (i * (resolution - 1) + j) + 5] = (i + 1) * resolution + j + 1;
		}

	return v;
}

std::vector<unsigned short> createLinePatchIndieces(int resolution) {
	int len = 2 * (resolution - 1);
	std::vector<unsigned short> v(len);

	for (uint16_t i = 0; i < (resolution - 1); i++) {
		v[2 * i] = i;
		v[2 * i + 1] = i + 1;
	}

	return v;
}