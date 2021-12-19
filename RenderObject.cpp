#include "RenderObject.hpp"
#include "VulkanSetup.hpp"
#include "Shader.hpp"
#include "Buffer.hpp"
#include "Texture.hpp"

extern PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
extern PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
extern PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
extern PFN_vkDestroyPipeline vkDestroyPipeline;
extern PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
extern PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
extern PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
extern PFN_vkMapMemory vkMapMemory;
extern PFN_vkUnmapMemory vkUnmapMemory;
extern PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
extern PFN_vkCmdBindPipeline vkCmdBindPipeline;
extern PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
extern PFN_vkCmdDrawIndexed vkCmdDrawIndexed;

RenderObject::RenderObject(VulkanSetup* _vkSetup, VkDescriptorPool _descriptorPool, json& gobj, Matrix& mView) : mView(mView) {
	vkSetup = _vkSetup;
	descriptorPool = _descriptorPool;
	vertexShader = new Shader(vkSetup, gobj["shader"]["vertexShaderFileName"].get<std::string>().c_str());
	fragmentShader = new Shader(vkSetup, gobj["shader"]["fragmentShaderFileName"].get<std::string>().c_str());
	vertexShader->load();
	fragmentShader->load();
	vertexShader->createShaderModule();
	fragmentShader->createShaderModule();
	vertexShader->createShaderStageInfo(VK_SHADER_STAGE_VERTEX_BIT);
	fragmentShader->createShaderStageInfo(VK_SHADER_STAGE_FRAGMENT_BIT);
	shaderStages = { vertexShader->getShaderStageInfo(), fragmentShader->getShaderStageInfo() };
	formats = { VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32_SFLOAT };
	offsets = { offsetof(Vertex, pos), offsetof(Vertex, color), offsetof(Vertex, texCoords) };
	topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	pTessellationStateCreateInfo = nullptr;
	vertexOffset = 0;
	firstIndex = 0;
	uboBufferSize = 0x200;
	mProj[0][0] = (float)vkSetup->getSwapChainExtent().height / (float)vkSetup->getSwapChainExtent().width * 0.5f;
	mProj[1][1] = 0.5f;
	color[0] = 0.0f; color[1] = 1.0f; color[2] = 0.0f; color[3] = 1.0f;
	texture = new Texture(vkSetup, gobj["texture"]["fileName"].get<std::string>().c_str());
	getAttributeDescriptions();
	getBindingDescription(sizeof(Vertex));
	createUniformBuffer();
	createPipelineLayout();
	createGraphicsPipeline();
	createDescriptorSet();
}

RenderObject::~RenderObject() {
	delete texture;
	vkFreeDescriptorSets(vkSetup->getDevice(), descriptorPool, 1, &descriptorSet);
	vkDestroyPipeline(vkSetup->getDevice(), graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(vkSetup->getDevice(), pipelineLayout, nullptr);
	delete uniformBuffer;
	delete fragmentShader;
	delete vertexShader;
}

void RenderObject::getAttributeDescriptions() {
	for (uint32_t i = 0; i < formats.size(); i++) {
		VkVertexInputAttributeDescription attributeDescription;
		attributeDescription.binding = 0;
		attributeDescription.location = i;
		attributeDescription.format = formats[i];
		attributeDescription.offset = offsets[i];
		attributeDescriptions.push_back(attributeDescription);
	}
}

void RenderObject::getBindingDescription(uint32_t stride) {
	bindingDescription = {};
	bindingDescription.binding = 0;
	bindingDescription.stride = stride;
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
}

void RenderObject::createUniformBuffer() {
	uniformBuffer = new Buffer(vkSetup->getPhysicalDevice(), vkSetup->getDevice(), vkSetup->getCommandPool(), vkSetup->getQueue());
	uniformBuffer->createBuffer(uboBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void RenderObject::createPipelineLayout() {
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pNext = nullptr;
	pipelineLayoutInfo.flags = 0;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = vkSetup->getDescriptorSetLayoutPtr();
	pipelineLayoutInfo.pushConstantRangeCount = (uint32_t)pushConstantRange.size();
	pipelineLayoutInfo.pPushConstantRanges = pushConstantRange.data();

	if (vkCreatePipelineLayout(vkSetup->getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		std::cout << "Failed to create pipeline layout!\n";
		exit(1);
	}
}

void RenderObject::createGraphicsPipeline() {	
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.pNext = nullptr;
	vertexInputInfo.flags = 0;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)attributeDescriptions.size();
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.pNext = nullptr;
	inputAssembly.flags = 0;
	inputAssembly.topology = topology;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)vkSetup->getSwapChainExtent().width;
	viewport.height = (float)vkSetup->getSwapChainExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = vkSetup->getSwapChainExtent();

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.pNext = nullptr;
	viewportState.flags = 0;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.pNext = nullptr;
	rasterizer.flags = 0;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 2.0f; // --> Feature muss extra aktiviert werden
	rasterizer.cullMode = VK_CULL_MODE_NONE; // Front and Back werden gerendert
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.pNext = nullptr;
	multisampling.flags = 0;
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {}; // Optional

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.pNext = nullptr;
	colorBlending.flags = 0;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = (uint32_t)shaderStages.size();
	pipelineInfo.pStages = shaderStages.data();
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pTessellationState = pTessellationStateCreateInfo;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr; // Optional
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = vkSetup->getRenderPass();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	if (vkCreateGraphicsPipelines(vkSetup->getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
		std::cout << "Failed to create graphics pipeline!\n";
		exit(1);
	}

	std::cout << "Graphics Pipeline created.\n";
}

void RenderObject::createDescriptorSet() {
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = vkSetup->getDescriptorSetLayoutPtr();

	if (vkAllocateDescriptorSets(vkSetup->getDevice(), &allocInfo, &descriptorSet) != VK_SUCCESS) {
		std::cout << "Failed to allocate descriptor set!\n";
		exit(1);
	}

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = uniformBuffer->getBuffer();
	bufferInfo.offset = 0;
	bufferInfo.range = 3 * sizeof(Matrix);

	VkDescriptorBufferInfo materialBufferInfo = {};
	materialBufferInfo.buffer = uniformBuffer->getBuffer();
	materialBufferInfo.offset = 0x100;
	materialBufferInfo.range = sizeof(color);

	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = texture->getTextureImageView();
	imageInfo.sampler = texture->getTextureSampler();

	std::vector<VkWriteDescriptorSet> descriptorWrites;

	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo = &bufferInfo;
	descriptorWrite.pImageInfo = nullptr; // Optional
	descriptorWrite.pTexelBufferView = nullptr; // Optional
	descriptorWrites.push_back(descriptorWrite);

	descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = 1;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pImageInfo = &imageInfo;
	descriptorWrites.push_back(descriptorWrite);

	descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = 2;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo = &materialBufferInfo;
	descriptorWrite.pImageInfo = nullptr; // Optional
	descriptorWrite.pTexelBufferView = nullptr; // Optional
	descriptorWrites.push_back(descriptorWrite);

	vkUpdateDescriptorSets(vkSetup->getDevice(), (uint32_t)descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
}

void RenderObject::createCommands(VkCommandBuffer cmdBuffer, VkBuffer* vertexBuffer) {
	VkDeviceSize offsets[] = { vertexOffset };
	vkCmdBindVertexBuffers(cmdBuffer, 0, 1, vertexBuffer, offsets);
	vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
	vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
	vkCmdDrawIndexed(cmdBuffer, indexCount, 1, firstIndex, 0, 0);
}

void RenderObject::updateUniformBuffer() {
	void* data;
	vkMapMemory(vkSetup->getDevice(), uniformBuffer->getBufferMemory(), 0, uboBufferSize, 0, &data);
	memcpy((char*)data, &mModel, sizeof(Matrix));
	memcpy((char*)data + sizeof(Matrix), &mView, sizeof(Matrix));
	memcpy((char*)data + 2 * sizeof(Matrix), &mProj, sizeof(Matrix));
	memcpy((char*)data + 0x100, &color, sizeof(color));
	vkUnmapMemory(vkSetup->getDevice(), uniformBuffer->getBufferMemory());
}

/* Setter */
void RenderObject::setVertexOffet(uint64_t _vertexOffset) { vertexOffset = _vertexOffset; }
void RenderObject::setIndexCount(uint32_t _indexCount) { indexCount = _indexCount; }
void RenderObject::setFirstIndex(uint32_t _firstIndex) { firstIndex = _firstIndex; }