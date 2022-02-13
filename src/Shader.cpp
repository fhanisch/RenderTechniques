#include "Shader.hpp"
#include "VulkanSetup.hpp"
#include <iostream>

extern PFN_vkCreateShaderModule vkCreateShaderModule;
extern PFN_vkDestroyShaderModule vkDestroyShaderModule;

Shader::Shader(VulkanSetup* _vkSetup, const char* fileName) {
	vkSetup = _vkSetup;
	file = fopen(fileName, "rb");
	if (!file) {
		std::cout << "Could not open '" << fileName << "'!" << std::endl;
		exit(1);
	}
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file);
	code = new unsigned int[size];
}

Shader::~Shader() {
	vkDestroyShaderModule(vkSetup->getDevice(), shaderModule, nullptr);
	delete[] code;
	fclose(file);
}

void Shader::load() {	
	fread(code, size, 1, file);
}

void Shader::createShaderModule() {
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.codeSize = size;
	createInfo.pCode = code; // ! alignment Anforderungen von uint32_t beachten !

	if (vkCreateShaderModule(vkSetup->getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		std::cout << "Failed to create shader module!\n";
		exit(1);
	}
}

void Shader::createShaderStageInfo(VkShaderStageFlagBits stage) {
	shaderStageInfo = {};
	shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfo.pNext = nullptr;
	shaderStageInfo.flags = 0;
	shaderStageInfo.stage = stage;
	shaderStageInfo.module = shaderModule;
	shaderStageInfo.pName = "main";
	shaderStageInfo.pSpecializationInfo = nullptr;
}

VkPipelineShaderStageCreateInfo Shader::getShaderStageInfo() { return shaderStageInfo; }