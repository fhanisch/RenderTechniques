#pragma once

#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR

#include <vulkan/vulkan.h>
#include <vector>
#include "Matrix.hpp"
#include "json.hpp"

using json = nlohmann::json;

typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];

struct Vertex {
	vec3 pos;
	vec3 normal;
	vec3 color;
	vec2 texCoords;
};

class VulkanSetup;
class Buffer;
class Shader;
class Texture;

class RenderObject {
	VulkanSetup* vkSetup;
	VkDescriptorPool descriptorPool;
	uint64_t vertexOffset;
	uint32_t indexCount;
	uint32_t firstIndex;
	Shader* vertexShader;
	Shader* fragmentShader;
	vec4 color;
	Texture* texture;
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
	std::vector<VkFormat> formats;
	std::vector<uint32_t> offsets;
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
	std::vector< VkPushConstantRange> pushConstantRange;
	VkVertexInputBindingDescription bindingDescription;
	VkPrimitiveTopology topology;
	VkPipelineTessellationStateCreateInfo* pTessellationStateCreateInfo;
	VkDeviceSize uboBufferSize;
	Buffer* uniformBuffer;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkDescriptorSet descriptorSet;
	Matrix& mView;
	void getAttributeDescriptions();
	void getBindingDescription(uint32_t stride);
	void createUniformBuffer();
	void createPipelineLayout();
	void createGraphicsPipeline();
	void createDescriptorSet();
public:
	Matrix mModel;
	Matrix mProj;
	RenderObject(VulkanSetup* _vkSetup, VkDescriptorPool _descriptorPool, json& gobj, Matrix& mView);
	~RenderObject();
	void createCommands(VkCommandBuffer cmdBuffer, VkBuffer* vertexBuffer);
	void updateUniformBuffer();
	/* Setter */
	void setVertexOffet(uint64_t _vertexOffset);
	void setIndexCount(uint32_t _indexCount);
	void setFirstIndex(uint32_t _firstIndex);
};