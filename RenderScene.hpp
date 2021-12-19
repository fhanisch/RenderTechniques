#pragma once

#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR

#include <vulkan/vulkan.h>
#include <vector>
#include "Matrix.hpp"
#include "json.hpp"

using json = nlohmann::json;

struct Vertex;
class VulkanSetup;
class RenderObject;
class Buffer;

class RenderScene {
	VulkanSetup* vkSetup;
	VkDescriptorPool descriptorPool;
	RenderObject** obj;
	Buffer* vertexBuffer;
	Buffer* indexBuffer;
	std::vector<VkCommandBuffer> commandBuffers;
	json graphicObjects;
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;
	Matrix mView;
	void createDescriptorPool();
	void createGraphicObjects();
	void createVertexBuffer();
	void createIndexBuffer();
	void createCommandBuffers();
public:
	RenderScene(VulkanSetup* _vkSetup, json& _graphicObjects);
	~RenderScene();
	void updateUniformBuffers();
	void camMotion();
	void drawFrame();
};