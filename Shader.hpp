#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR

#include <vulkan/vulkan.h>
#include <stdio.h>

class VulkanSetup;

class Shader {
	VulkanSetup* vkSetup;
	FILE* file;
	size_t size;
	unsigned int* code;	
	VkShaderModule shaderModule;
	VkPipelineShaderStageCreateInfo shaderStageInfo;
public:
	Shader(VulkanSetup* _vkSetup, const char* fileName);
	~Shader();
	void load();
	void createShaderModule();
	void createShaderStageInfo(VkShaderStageFlagBits stage);
	VkPipelineShaderStageCreateInfo getShaderStageInfo();
};