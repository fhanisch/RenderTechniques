#pragma once

#include "Matrix.hpp"
#include <time.h>

class RenderObject;

namespace Models {

	class Model {
	public:
		RenderObject* obj;
		Model(RenderObject* _graphicObj);
	};

	class Square : public Model {
		Vector pos;
	public:
		Square(RenderObject* _graphicObj);
		void motion();
	};

	class Tacho : public Model {
	public:
		Tacho(RenderObject* _graphicObj);
	};

	class FilledCircle : public Model {
		Matrix t;
	public:
		FilledCircle(RenderObject* _graphicObj);
		void motion();
	};

	class FlatPerlin2d : public Model {
	public:
		FlatPerlin2d(RenderObject* _graphicObj);
	};

	class Star : public Model {
		Vector pos;
		Matrix t;
		Matrix r;
	public:
		Star(RenderObject* _graphicObj);
		void motion();
	};

	class Perlin1D : public Model {
	public:
		Perlin1D(RenderObject* _graphicObj);
	};

	class PerlinCircle : public Model {
		clock_t startTime;
	public:
		PerlinCircle(RenderObject* _graphicObj);
		void motion();
	};

	class Welle : public Model {
		clock_t startTime;
	public:
		Welle(RenderObject* _graphicObj);
		void motion();
	};

	class Plane : public Model {
	public:
		Plane(RenderObject* _graphicObj);
	};

	class Sphere : public Model {
	public:
		Sphere(RenderObject* _graphicObj);
	};

	class TestObject : public Model {
	public:
		TestObject(RenderObject* _graphicObj);
	};

	class CurveTessellator : public Model {
	public:
		CurveTessellator(RenderObject* _graphicObj);
	};

	class Perlin1dTessellator : public Model {
	public:
		Perlin1dTessellator(RenderObject* _graphicObj);
	};

	class Cube : public Model {
	public:
		Cube(RenderObject* _graphicObj);
	};

	class CubeSphere : public Model {
	public:
		CubeSphere(RenderObject* _graphicObj);
	};

	class Planet : public Model {
	public:
		Planet(RenderObject* _graphicObj);
	};

	class Planet2 : public Model {
	public:
		Planet2(RenderObject* _graphicObj);
	};

	class Perlin1DVertices : public Model {
	public:
		Perlin1DVertices(RenderObject* _graphicObj);
	};
}