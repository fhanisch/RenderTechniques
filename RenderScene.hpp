#pragma once

#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR

#include <vulkan/vulkan.h>
#include <vector>
#include "Matrix.hpp"
#include "json.hpp"
#include "Models.hpp"
#include "GeometryData.hpp"

using json = nlohmann::json;
using namespace Models;

struct Vertex;
class VulkanSetup;
class RenderObject;
class Buffer;

class RenderScene {
	VulkanSetup* vkSetup;
	VkDescriptorPool descriptorPool;
	std::vector<RenderObject*> obj;
	Buffer* vertexBuffer;
	Buffer* indexBuffer;
	std::vector<VkCommandBuffer> commandBuffers;
	GeometryData geoData;
	json graphicObjects;
	Matrix cam1;
	Matrix mView;
	Matrix mView3D;
	Star* star;
	Square* square;
	Tacho* tacho;
	FilledCircle* filledCircle;
	FlatPerlin2d* flatPerlin2d;
	Perlin1D* perlin1D;
	PerlinCircle* perlinCircle;
	Welle* welle;
	Plane* plane;
	Sphere* sphere;
	TestObject* testObj;
	CurveTessellator* curveTessellator;
	Perlin1dTessellator* perlin1dTessellator;
	Cube* cube;
	CubeSphere* cubeSphere;
	Planet* planet;
	bool updateAll = true;
	void createDescriptorPool();
	void createGeometryData();
	void createGraphicObjects();
	void createVertexBuffer();
	void createIndexBuffer();
	void createCommandBuffers();
public:
	RenderScene(VulkanSetup* _vkSetup, json& _graphicObjects);
	~RenderScene();
	void assembleScene();
	void updateUniformBuffers();
	void camMotion();
	void drawFrame();
};