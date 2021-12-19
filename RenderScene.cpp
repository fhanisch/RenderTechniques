#include "RenderScene.hpp"
#include "RenderObject.hpp"
#include "VulkanSetup.hpp"
#include "Buffer.hpp"
#include <iostream>

#define KEY_ESC VK_ESCAPE
#define KEY_SPACE VK_SPACE
#define KEY_LEFT VK_LEFT
#define KEY_RIGHT VK_RIGHT
#define KEY_UP VK_UP
#define KEY_DOWN VK_DOWN
#define KEY_SHIFT VK_SHIFT
#define KEY_A 0x41
#define KEY_D 0x44
#define KEY_W 0x57
#define KEY_S 0x53
#define KEY_X 0x58
#define KEY_Y 0x59

extern PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
extern PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
extern PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
extern PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
extern PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
extern PFN_vkEndCommandBuffer vkEndCommandBuffer;
extern PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
extern PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
extern PFN_vkCmdPushConstants vkCmdPushConstants;
extern PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
extern PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
extern PFN_vkCmdBindPipeline vkCmdBindPipeline;
extern PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
extern PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
extern PFN_vkCmdDraw vkCmdDraw;
extern PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
extern PFN_vkQueueSubmit vkQueueSubmit;
extern PFN_vkQueueWaitIdle vkQueueWaitIdle;
extern PFN_vkQueuePresentKHR vkQueuePresentKHR;

extern bool key[256];

RenderScene::RenderScene(VulkanSetup* _vkSetup, json& _graphicObjects) {
	vkSetup = _vkSetup;
	graphicObjects = _graphicObjects;
	createDescriptorPool();
	createGraphicObjects();
	createVertexBuffer();
	createIndexBuffer();
	createCommandBuffers();
}

RenderScene::~RenderScene() {
	vkFreeCommandBuffers(vkSetup->getDevice(), vkSetup->getCommandPool(), (uint32_t)commandBuffers.size(), commandBuffers.data());
	delete indexBuffer;
	delete vertexBuffer;
	for (uint32_t i = 0; i < graphicObjects.size(); i++) {
		delete obj[i];
	}
	vkDestroyDescriptorPool(vkSetup->getDevice(), descriptorPool, nullptr);
}

void RenderScene::createDescriptorPool() {
	VkDescriptorPoolSize poolSize[2] = {};
	poolSize[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize[0].descriptorCount = (static_cast<uint32_t>(graphicObjects.size()) + 1) * 2;
	poolSize[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSize[1].descriptorCount = static_cast<uint32_t>(graphicObjects.size()) + 1;

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.poolSizeCount = 2;
	poolInfo.pPoolSizes = poolSize;
	poolInfo.maxSets = static_cast<uint32_t>(graphicObjects.size()) + 1;

	if (vkCreateDescriptorPool(vkSetup->getDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		std::cout << "Failed to create descriptor pool!\n";
		exit(1);
	}
	std::cout << "Descriptor pool created.\n";
}

void RenderScene::createGraphicObjects() {
	Vertex vertex;
	std::cout << "Number of GraphicObjects: " << graphicObjects.size() << std::endl;
	obj = new RenderObject*[graphicObjects.size()];
	for (uint32_t i = 0; i < graphicObjects.size(); i++) {
		json gobj = graphicObjects[i];
		std::string name = gobj["name"].get<std::string>();
		std::cout << "Object 1: " << name << std::endl;
		obj[i] = new RenderObject(vkSetup, descriptorPool, gobj, mView);
		obj[i]->setVertexOffet(vertices.size() * sizeof(Vertex));
		obj[i]->setFirstIndex((uint32_t)indices.size());
		json verts = gobj["vertices"];
		for (uint32_t i = 0; i < verts.size(); i++) {
			std::vector<float> pos = verts[i]["position"].get<std::vector<float>>();
			std::vector<float> normal = verts[i]["normal"].get<std::vector<float>>();
			std::vector<float> color = verts[i]["color"].get<std::vector<float>>();
			std::vector<float> texCoords = verts[i]["texCoords"].get<std::vector<float>>();
			memcpy(&vertex.pos, pos.data(), pos.size() * sizeof(float));
			memcpy(&vertex.normal, normal.data(), normal.size() * sizeof(float));
			memcpy(&vertex.color, color.data(), color.size() * sizeof(float));
			memcpy(&vertex.texCoords, texCoords.data(), texCoords.size() * sizeof(float));
			vertices.push_back(vertex);
		}
		std::vector<uint16_t> inds = gobj["indices"].get<std::vector<uint16_t>>();
		obj[i]->setIndexCount((uint32_t)inds.size());
		for (uint32_t i = 0; i < inds.size(); i++) {
			indices.push_back(inds[i]);
		}
	}
}

void RenderScene::createVertexBuffer() {
	vertexBuffer = new Buffer(vkSetup->getPhysicalDevice(), vkSetup->getDevice(), vkSetup->getCommandPool(), vkSetup->getQueue());
	vertexBuffer->createDeviceLocalBuffer(vertices.data(), vertices.size()*sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
}

void RenderScene::createIndexBuffer() {
	indexBuffer = new Buffer(vkSetup->getPhysicalDevice(), vkSetup->getDevice(), vkSetup->getCommandPool(), vkSetup->getQueue());
	indexBuffer->createDeviceLocalBuffer(indices.data(), indices.size()*sizeof(uint16_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);	
}

void RenderScene::createCommandBuffers() {
	commandBuffers = std::vector<VkCommandBuffer>(vkSetup->getSwapChainImagesCount());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = vkSetup->getCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(vkSetup->getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		std::cout << "failed to allocate command buffers!\n";
		exit(1);
	}

	for (uint32_t i = 0; i < commandBuffers.size(); i++) {
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr; // Optional

		vkBeginCommandBuffer(commandBuffers[i], &beginInfo);
		{
			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = vkSetup->getRenderPass();
			renderPassInfo.framebuffer = vkSetup->getSwapChainFrameBuffers()[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = vkSetup->getSwapChainExtent();
			VkClearValue clearValues[2] = {};
			clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
			clearValues[1].depthStencil = { 1.0f, 0 };
			renderPassInfo.clearValueCount = 2;
			renderPassInfo.pClearValues = clearValues;

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			{
				//vkCmdPushConstants(commandBuffers[i], obj[4]->getPipelineLayout(), obj[4]->getPushConstantRange()->stageFlags, obj[4]->getPushConstantRange()->offset, obj[4]->getPushConstantRange()->size, ((PerlinCircle*)obj[4])->getPushConstants());

				VkBuffer vB[] = { vertexBuffer->getBuffer() };
				vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT16);

				for (uint32_t j = 0; j < graphicObjects.size(); j++) {
					obj[j]->createCommands(commandBuffers[i], vB);
				}
/*
				vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, txtObj->getGraphicsPipeline());
				vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, txtObj->getPipelineLayout(), 0, 1, txtObj->getDescriptorSetPtr(), 0, nullptr);
				VkDeviceSize offsets1 = 0;
				VkBuffer vB1[] = { txtObj->getTextOverlayVertexBuffer() };
				vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vB1, &offsets1);
				for (uint32_t j = 0; j < txtObj->getNumLetters(); j++) {
					vkCmdDraw(commandBuffers[i], 4, 1, j * 4, 0);
				}
*/
			}
			vkCmdEndRenderPass(commandBuffers[i]);
		}
		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
			std::cout << "Failed to record command buffer!\n";
			exit(1);
		}
	}
	std::cout << "Command buffers created.\n";
}

void RenderScene::updateUniformBuffers() {
	for (uint32_t i = 0; i < graphicObjects.size(); i++) {
		obj[i]->updateUniformBuffer();
	}
}

void RenderScene::camMotion() {
	static float phi = 0.0f;
	static float x = 0.0f;
	static float x2 = 0.0f;

	if (key[KEY_A]) {
		phi -= 0.005f;
		mView.rotZ(phi);
	}
	if (key[KEY_D]) {
		phi += 0.005f;
		mView.rotZ(phi);
	}
	if (key[KEY_LEFT] && !key[KEY_SHIFT]) {
		x -= 0.001f;
		obj[1]->mModel.translate(Vec(x, 0.0f, 0.0f, 1.0f));
	}
	if (key[KEY_RIGHT] && !key[KEY_SHIFT]) {
		x += 0.001f;
		obj[1]->mModel.translate(Vec(x, 0.0f, 0.0f, 1.0f));
	}
	if (key[KEY_LEFT] && key[KEY_SHIFT]) {
		x2 -= 0.001f;
		obj[2]->mModel.translate(Vec(x2, 0.0f, 0.0f, 1.0f));
	}
	if (key[KEY_RIGHT] && key[KEY_SHIFT]) {
		x2 += 0.001f;
		obj[2]->mModel.translate(Vec(x2, 0.0f, 0.0f, 1.0f));
	}
}

#undef max
void RenderScene::drawFrame() {
	uint32_t imageIndex;
	vkAcquireNextImageKHR(vkSetup->getDevice(), vkSetup->getSwapChain(), std::numeric_limits<uint64_t>::max(), vkSetup->getImageAvailableSemaphore(), VK_NULL_HANDLE, &imageIndex);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	VkSemaphore waitSemaphores[] = { vkSetup->getImageAvailableSemaphore() };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
	VkSemaphore signalSemaphores[] = { vkSetup->getRenderFinishedSemaphore() };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(vkSetup->getQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
		std::cout << "failed to submit draw command buffer!\n";
		exit(1);
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = { vkSetup->getSwapChain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(vkSetup->getQueue(), &presentInfo);

	vkQueueWaitIdle(vkSetup->getQueue()); // Synchronisation der App mit der GPU (nicht notwendig)
}