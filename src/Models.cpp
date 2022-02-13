#include "Models.hpp"
#include "RenderObject.hpp"

#define KEY_ESC VK_ESCAPE
#define KEY_SPACE VK_SPACE
#define KEY_LEFT VK_LEFT
#define KEY_RIGHT VK_RIGHT
#define KEY_UP VK_UP
#define KEY_DOWN VK_DOWN
#define KEY_SHIFT VK_SHIFT
#define KEY_CONTROL VK_CONTROL
#define KEY_A 0x41
#define KEY_D 0x44
#define KEY_W 0x57
#define KEY_S 0x53
#define KEY_X 0x58
#define KEY_Y 0x59
#define KEY_R 0x52
#define KEY_T 0x54

extern bool key[256];

using namespace Models;

Model::Model(RenderObject* _graphicObj) {
	obj = _graphicObj;
}

Square::Square(RenderObject* _graphicObj) : Model(_graphicObj) {
	
}

void Square::motion() {
	static float phi;
	if (key[KEY_R]) {
		phi -= 0.005f;
		obj->update = true;
	}
	if (key[KEY_T]) {
		phi += 0.005f;
		obj->update = true;
	}
	if (obj->update) obj->mModel.rotZ(phi);
}

Tacho::Tacho(RenderObject* _graphicObj) : Model(_graphicObj) {
	obj->mModel = Matrix().translate(Vector(0.0f, 5.0f, 0.2f, 1.0f));
}

FilledCircle::FilledCircle(RenderObject* _graphicObj) : Model(_graphicObj) {
	obj->mModel = Matrix().translate(Vector(0.0f, 5.0f, 0.1f, 1.0f));
}

void FilledCircle::motion() {
	float dx = 0.0f;
	float dy = 0.0f;
	if (key[KEY_LEFT] && key[KEY_CONTROL]) {
		dx = -0.005f;
		obj->update = true;
	}
	if (key[KEY_RIGHT] && key[KEY_CONTROL]) {
		dx = 0.005f;
		obj->update = true;
	}
	if (key[KEY_UP] && key[KEY_CONTROL]) {
		dy = -0.005f;
		obj->update = true;
	}
	if (key[KEY_DOWN] && key[KEY_CONTROL]) {
		dy = 0.005f;
		obj->update = true;
	}
	if (obj->update) obj->mModel = obj->mModel * t.translate(Vector(dx, dy, 0.0f, 1.0f));
}

FlatPerlin2d::FlatPerlin2d(RenderObject* _graphicObj) : Model(_graphicObj) {
	obj->mModel = Matrix().translate(Vector(-5.0f, 0.0f, 0.2f, 1.0f));
}

Star::Star(RenderObject* _graphicObj) : Model(_graphicObj) {
	obj->mModel.translate(Vector(2.0f, 0.0f, 0.0f, 1.0f));
	pos = Vector(2.0f, 0.0f, 0.0f, 1.0f);
	t.translate(Vector(2.0f, 0.0f, 0.0f, 1.0f));
}

void Star::motion() {
	static float theta = 0.0f;
	theta += 0.005f;
	obj->mModel = r.rotZ(theta) * t;
	obj->update = true;
}

Perlin1D::Perlin1D(RenderObject* _graphicObj) : Model(_graphicObj) {
	obj->mModel = Matrix().translate(Vector(0.0f, -5.0f, 0.0f, 1.0f)) * Matrix().scale(Vector(5.0f, 1.0f, 1.0f, 1.0f));
}

PerlinCircle::PerlinCircle(RenderObject* _graphicObj) : Model(_graphicObj) {
	obj->mModel.scale(Vector(2.0f, 2.0f, 0.0f, 1.0f));
	startTime = clock();
}

void PerlinCircle::motion() {
	if (key[KEY_SPACE]) {
		float seed_u = (float)(clock() - startTime) / CLOCKS_PER_SEC;
		obj->setPushConstants(seed_u, seed_u + 245.0f);
	}
}

Welle::Welle(RenderObject* _graphicObj) : Model(_graphicObj) {
	startTime = clock();
	obj->mModel = Matrix().translate(Vector(0.0f, -2.5f, 0.2f, 1.0f)) * Matrix().scale(Vector(4.0f, 2.0f, 1.0f, 1.0f));
}

void Welle::motion() {
	float time = (float)(clock() - startTime) / CLOCKS_PER_SEC;
	obj->myValue.x = time;
	obj->update = true;
}

Plane::Plane(RenderObject* _graphicObj) : Model(_graphicObj) {
	obj->mProj.frustum(0.25f * (float)obj->swapChainExtent.width / (float)obj->swapChainExtent.height, 0.25f, 0.5f, 1000.0f);
	obj->mModel = Matrix().translate(Vector(0.0f, 105.5f, -1.0f, 1.0f)) * Matrix().rotX(PI / 2.0f) * Matrix().scale(Vector(10.0f, 10.0f, 1.0f, 1.0f));
	obj->myValue = Vector(1.0);
}

Sphere::Sphere(RenderObject* _graphicObj) : Model(_graphicObj) {
	obj->mProj.frustum(0.25f * (float)obj->swapChainExtent.width / (float)obj->swapChainExtent.height, 0.25f, 0.5f, 1000.0f);
	obj->mModel = Matrix().rotY(0.8f * PI / 2.0f) * Matrix().rotZ(PI / 2.0f) * Matrix().translate(Vector(0.0f, 101.0f, 0.0f, 1.0f));
	obj->myValue = Vector(1.0f, 0.0f, 1.0f, 1.0f);
}

TestObject::TestObject(RenderObject* _graphicObj) : Model(_graphicObj) {
	obj->mProj.frustum(0.25f * (float)obj->swapChainExtent.width / (float)obj->swapChainExtent.height, 0.25f, 0.5f, 1000.0f);
	obj->mModel = Matrix().translate(Vector(0.0f, 110.0f, 0.0f, 1.0f));
	obj->myValue = Vector(1.0f, 1.0f, 0.0f, 1.0f);
}

CurveTessellator::CurveTessellator(RenderObject* _graphicObj) : Model(_graphicObj) {
	obj->mModel = Matrix().translate(Vector(-5.0f, -5.0f, 0.0f, 1.0f));
	obj->myValue = Vector(1.0f, 1.0f, 0.0f, 1.0f);
}

Perlin1dTessellator::Perlin1dTessellator(RenderObject* _graphicObj) : Model(_graphicObj) {
	obj->mModel = Matrix().translate(Vector(5.0f, 0.0f, 0.2f, 1.0f)) * Matrix().scale(Vector(2.0f, 1.0f, 1.0f, 1.0f));
	obj->mProj[0][0] = (float)obj->swapChainExtent.height / (float)obj->swapChainExtent.width;
	obj->myValue = Vector(1.0f, 0.0f, 0.0f, 1.0f);
}

Cube::Cube(RenderObject* _graphicObj) : Model(_graphicObj) {
	obj->mProj.frustum(0.25f * (float)obj->swapChainExtent.width / (float)obj->swapChainExtent.height, 0.25f, 0.5f, 1000.0f);
	obj->mModel = Matrix().rotY(-PI / 4.0f) * Matrix().rotZ(-PI / 4.0f) * Matrix().translate(Vector(0.0f, 110.0f, 0.0f, 1.0f));
	obj->myValue = Vector(1.0f, 1.0f, 0.0f, 1.0f);
}

CubeSphere::CubeSphere(RenderObject* _graphicObj) : Model(_graphicObj) {
	obj->mProj.frustum(0.25f * (float)obj->swapChainExtent.width / (float)obj->swapChainExtent.height, 0.25f, 0.5f, 1000.0f);
	obj->mModel = Matrix().rotY(PI / 2.0f) * Matrix().rotZ(PI) * Matrix().translate(Vector(0.0f, 102.0f, 0.0f, 1.0f));
	obj->myValue = Vector(0.0f, 0.0f, 1.0f, 1.0f);
}

Planet::Planet(RenderObject* _graphicObj) : Model(_graphicObj) {
	obj->mProj.frustum(0.25f * (float)obj->swapChainExtent.width / (float)obj->swapChainExtent.height, 0.25f, 0.5f, 1000.0f);
	obj->mModel = Matrix().rotX(PI / 8.0f) * Matrix().translate(Vector(0.0f, 120.0f, 0.0f, 1.0f));
}

Planet2::Planet2(RenderObject* _graphicObj) : Model(_graphicObj) {
	obj->mProj.frustum(0.25f * (float)obj->swapChainExtent.width / (float)obj->swapChainExtent.height, 0.25f, 0.5f, 1000.0f);
	obj->myValue.x = 100.0f;
}

Perlin1DVertices::Perlin1DVertices(RenderObject* _graphicObj) : Model(_graphicObj) {
	obj->mModel = Matrix().translate(Vector(0.0f, -5.0f, 0.0f, 1.0f)) * Matrix().scale(Vector(2.0f, 2.0f, 1.0f, 1.0f));
}

PerlinCircleVertices::PerlinCircleVertices(RenderObject* _graphicObj) : Model(_graphicObj) {
	obj->mModel = Matrix().translate(Vector(0.0f, -10.0, 0.0f, 1.0f)) * Matrix().scale(Vector(1.5f, 1.5f, 1.0f, 1.0f));
	obj->myValue = Vector(1.0f, 0.0f, 1.0f, 1.0f);
}

Terrain2D::Terrain2D(RenderObject* _graphicObj) : Model(_graphicObj) {
	obj->mProj.frustum(0.25f * (float)obj->swapChainExtent.width / (float)obj->swapChainExtent.height, 0.25f, 0.5f, 1000.0f);
	obj->mModel = Matrix().translate(Vector(0.0f, 106.0f, -40.0f, 1.0f)) * Matrix().scale(Vector(20.0f, 20.0f, 20.0f, 1.0f));
	obj->myValue = Vector(1.0f, 1.0f, 0.0f, 1.0f);
}