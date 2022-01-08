#pragma once

#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR

#include <vulkan/vulkan.h>
#include <vector>
#include "Matrix.hpp"
#include "json.hpp"

using json = nlohmann::json;

struct Vertex {
	Vector3 pos;
	Vector3 normal;
	Vector3 color;
	Vector2 texCoords;
};

struct Mesh {
	Vector2 uv;
	Vector3 normal;
};

class VulkanSetup;
class Buffer;
class Shader;
class Texture;

class RenderObject {
	std::string name;
	VulkanSetup* vkSetup;
	VkDescriptorPool descriptorPool;
	uint64_t vertexOffset;
	uint32_t indexCount;
	uint32_t firstIndex;
	Shader* vertexShader;
	Shader* tessellationControlShader;
	Shader* tessellationEvaluationShader;
	Shader* fragmentShader;
	Texture* texture;
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
	uint32_t stride;
	std::vector<VkFormat> formats;
	std::vector<uint32_t> offsets;
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
	std::vector<VkPushConstantRange> pushConstantRange;
	VkVertexInputBindingDescription bindingDescription;
	VkPrimitiveTopology topology;
	VkPipelineTessellationStateCreateInfo* pTessellationStateCreateInfo;
	VkDeviceSize uboBufferSize;
	Buffer* uniformBuffer;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkDescriptorSet descriptorSet;
	Matrix* mView;
	struct PushConstants {
		float seed_u;
		float seed_v;
	} pushConstants;
	void getAttributeDescriptions();
	void getBindingDescription();
	void createTessellationStateCreateInfo(uint32_t patchControlPoints);
	void createPushConstantRange(VkShaderStageFlags shaderStageFlags, uint32_t size);
	void createUniformBuffer();
	void createPipelineLayout();
	void createGraphicsPipeline();
	void createDescriptorSet();
public:
	VkExtent2D swapChainExtent;
	bool update = true;
	Matrix mModel;
	Matrix mProj;
	Vector myValue;
	RenderObject(VulkanSetup* _vkSetup, VkDescriptorPool _descriptorPool, json& gobj, Matrix* mView);
	~RenderObject();
	void createCommands(VkCommandBuffer cmdBuffer, VkBuffer* vertexBuffer);
	void updateUniformBuffer();
	/* Setter */
	std::string& getName();
	void setVertexOffet(uint64_t _vertexOffset);
	void setIndexCount(uint32_t _indexCount);
	void setFirstIndex(uint32_t _firstIndex);
	void setPushConstants(float seed_u, float seed_v);
	void setViewMatrix(Matrix* _mView);
};