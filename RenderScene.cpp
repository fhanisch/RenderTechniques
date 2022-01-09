#include "RenderScene.hpp"
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
#define KEY_CONTROL VK_CONTROL
#define KEY_A 0x41
#define KEY_D 0x44
#define KEY_W 0x57
#define KEY_S 0x53
#define KEY_X 0x58
#define KEY_Y 0x59
#define KEY_R 0x52
#define KEY_T 0x54

extern PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
extern PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
extern PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
extern PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
extern PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
extern PFN_vkEndCommandBuffer vkEndCommandBuffer;
extern PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
extern PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
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
	camIndex = 0;
	createDescriptorPool();
	createGeometryData();
	createGraphicObjects();
	createVertexBuffer();
	createIndexBuffer();
	createCommandBuffers();
}

RenderScene::~RenderScene() {
	vkFreeCommandBuffers(vkSetup->getDevice(), vkSetup->getCommandPool(), (uint32_t)commandBuffers.size(), commandBuffers.data());
	delete indexBuffer;
	delete vertexBuffer;
	for (uint32_t i = 0; i < obj.size(); i++) {
		delete obj[i];
	}
	vkDestroyDescriptorPool(vkSetup->getDevice(), descriptorPool, nullptr);
}

void RenderScene::createDescriptorPool() {
	VkDescriptorPoolSize poolSize[2] = {};
	poolSize[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize[0].descriptorCount = (static_cast<uint32_t>(graphicObjects.size())) * 2;
	poolSize[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSize[1].descriptorCount = static_cast<uint32_t>(graphicObjects.size());

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.poolSizeCount = 2;
	poolInfo.pPoolSizes = poolSize;
	poolInfo.maxSets = static_cast<uint32_t>(graphicObjects.size());

	if (vkCreateDescriptorPool(vkSetup->getDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		std::cout << "Failed to create descriptor pool!\n";
		exit(1);
	}
	std::cout << "Descriptor pool created.\n";
}

void RenderScene::createGeometryData() {
	geoData.createData("Plane", GEO_PLANE);
	geoData.createData("Star", GEO_STAR);
	geoData.createData("Perlin1D", GEO_MESH_1D, 1001);
	geoData.createData("Sphere", GEO_MESH_2D, 101);
	geoData.createData("CurvePatches", GEO_CURVE_PATCHES);
	geoData.createData("1D-Patches", GEO_PATCH_1D, 101);
	geoData.createData("Cube", GEO_CUBE);
	geoData.createData("CubeSphere", GEO_CUBE_SPHERE);
	geoData.createData("CubeSpherePatches", GEO_CUBE_SPHERE_PATCHES, 21);
	geoData.createData("CubeSphereTerrain", GEO_CUBE_SPHERE_TERRAIN, 3);
	geoData.createData("Perlin1DVertices", GEO_PERLIN_1D, 1001);
	geoData.createData("PerlinCircleVertices", GEO_PERLIN_CIRCLE, 1001);
	geoData.createData("Terrain2D", GEO_TERRAIN_2D, 101);
}

void RenderScene::createGraphicObjects() {
	Descriptor d;
	std::cout << "Number of GraphicObjects: " << graphicObjects.size() << std::endl;
	for (uint32_t i = 0; i < graphicObjects.size(); i++) {
		json gobj = graphicObjects[i];
		std::string name = gobj["name"].get<std::string>();
		std::cout << "Object " << i + 1 << ": " << name << std::endl;
		GeometryForm form = gobj["GeometryForm"];
		RenderObject* o = new RenderObject(vkSetup, descriptorPool, gobj, &mView);
		d = geoData[form];
		o->setVertexOffet(d.vertexOffset);
		o->setFirstIndex(d.firstIndex);
		o->setIndexCount(d.indexCount);
		obj.push_back(o);
	}
}

void RenderScene::createVertexBuffer() {
	vertexBuffer = new Buffer(vkSetup->getPhysicalDevice(), vkSetup->getDevice(), vkSetup->getCommandPool(), vkSetup->getQueue());
	vertexBuffer->createDeviceLocalBuffer(geoData.getVertices(), geoData.getVerticesSize() * sizeof(float), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
}

void RenderScene::createIndexBuffer() {
	indexBuffer = new Buffer(vkSetup->getPhysicalDevice(), vkSetup->getDevice(), vkSetup->getCommandPool(), vkSetup->getQueue());
	indexBuffer->createDeviceLocalBuffer(geoData.getIndices(), geoData.getIndicesSize() * sizeof(uint16_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
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

				for (uint32_t j = 0; j < obj.size(); j++) {
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

void RenderScene::assembleScene() {
	for (int i = 0; i < obj.size(); i++) {
		if (obj[i]->getName() == "Square") square = new Square(obj[i]);
		else if (obj[i]->getName() == "Star") star = new Star(obj[i]);
		else if (obj[i]->getName() == "Tacho") tacho = new Tacho(obj[i]);
		else if (obj[i]->getName() == "FilledCircle") filledCircle = new FilledCircle(obj[i]);
		else if (obj[i]->getName() == "FlatPerlin2d") flatPerlin2d = new FlatPerlin2d(obj[i]);
		else if (obj[i]->getName() == "Perlin1D") perlin1D = new Perlin1D(obj[i]);
		else if (obj[i]->getName() == "PerlinCircle") perlinCircle = new PerlinCircle(obj[i]);
		else if (obj[i]->getName() == "Welle") welle = new Welle(obj[i]);
		else if (obj[i]->getName() == "CurveTessellator") curveTessellator = new CurveTessellator(obj[i]);
		else if (obj[i]->getName() == "Perlin1dTessellator") perlin1dTessellator = new Perlin1dTessellator(obj[i]);
		else if (obj[i]->getName() == "Perlin1DVertices") perlin1DVertices = new Perlin1DVertices(obj[i]);
		else if (obj[i]->getName() == "PerlinCircleVertices") perlinCircleVertices = new PerlinCircleVertices(obj[i]);
		else if (obj[i]->getName() == "Plane") {
			plane = new Plane(obj[i]);
			plane->obj->setViewMatrix(&mView3D);
		}
		else if (obj[i]->getName() == "Sphere") {
			sphere = new Sphere(obj[i]);
			sphere->obj->setViewMatrix(&mView3D);
		}
		else if (obj[i]->getName() == "TestObject") {
			testObj = new TestObject(obj[i]);
			testObj->obj->setViewMatrix(&mView3D);
		}
		else if (obj[i]->getName() == "Cube") {
			cube = new Cube(obj[i]);
			cube->obj->setViewMatrix(&mView3D);
		}
		else if (obj[i]->getName() == "CubeSphere") {
			cubeSphere = new CubeSphere(obj[i]);
			cubeSphere->obj->setViewMatrix(&mView3D);
		}
		else if (obj[i]->getName() == "Planet") {
			planet = new Planet(obj[i]);
			planet->obj->setViewMatrix(&mView3D);
		}
		else if (obj[i]->getName() == "Planet2") {
			planet2 = new Planet2(obj[i]);
			planet2->obj->setViewMatrix(&mView3D);
		}
		else if (obj[i]->getName() == "Terrain2D") {
			terrain2D = new Terrain2D(obj[i]);
			terrain2D->obj->setViewMatrix(&mView3D);
		}
	}
	cam[camIndex].OP.translate({ 0.0f, 106.5f, 0.0f, 1.0f });
	cam[camIndex].elevation = 106.5f;
	cam[camIndex].motionType = ROTATORY; //TRANSLATIONAL;//ROTATORY;
	mView3D = cam[camIndex].OP.invert();
}

void RenderScene::updateUniformBuffers() {
	for (uint32_t i = 0; i < obj.size(); i++) {
		if (obj[i]->update || updateAll) {
			obj[i]->updateUniformBuffer();
			obj[i]->update = false;
		}
	}
	updateAll = false;
}

void RenderScene::camMotion() {	
	static float posX = 0.0f;
	static float posY = 0.0f;
	float deltaT = 0.02f;
	float deltaR = 0.01f;
	float dPhi = 0.0f;
	float dTheta = 0.0f;
	float dPsi = 0.0f;
	Matrix t, rx, ry, rz, rx2;

	/* 2D Cam-Motion */
	if (key[KEY_LEFT] && key[KEY_SHIFT]) {
		posX += deltaT;
		updateAll = true;
	}
	if (key[KEY_RIGHT] && key[KEY_SHIFT]) {
		posX -= deltaT;
		updateAll = true;
	}
	if(key[KEY_UP] && key[KEY_SHIFT]) {
		posY += deltaT;
		updateAll = true;
	}
	if (key[KEY_DOWN] && key[KEY_SHIFT]) {
		posY -= deltaT;
		updateAll = true;
	}
	if (updateAll) mView.translate(Vector(posX, posY, 0.0f, 1.0f));

	/* 3D Cam-Motion */
	if (cam[camIndex].motionType == TRANSLATIONAL) {

		if (key[KEY_A]) {
			t[3][0] = -deltaT;
			updateAll = true;
		}
		if (key[KEY_D]) {
			t[3][0] = deltaT;
			updateAll = true;
		}
		if (key[KEY_W]) {
			t[3][2] = -deltaT;
			updateAll = true;
		}
		if (key[KEY_S]) {
			t[3][2] = deltaT;
			updateAll = true;
		}
		if (key[KEY_X]) {
			t[3][1] = -deltaT;
			updateAll = true;
		}
		if (key[KEY_Y]) {
			t[3][1] = deltaT;
			updateAll = true;
		}
		if (key[KEY_LEFT] && !key[KEY_SHIFT] && !key[KEY_CONTROL]) {
			dPhi = deltaR;
			updateAll = true;
		}
		if (key[KEY_RIGHT] && !key[KEY_SHIFT] && !key[KEY_CONTROL]) {
			dPhi = -deltaR;
			updateAll = true;
		}
		if (key[KEY_UP] && !key[KEY_SHIFT] && !key[KEY_CONTROL]) {
			dTheta = -deltaR;
			updateAll = true;
		}
		if (key[KEY_DOWN] && !key[KEY_SHIFT] && !key[KEY_CONTROL]) {
			dTheta = deltaR;
			updateAll = true;
		}
		if (updateAll) {			
			cam[camIndex].OP = cam[camIndex].OP.position() * ry.rotY(dPhi) * cam[camIndex].OP.orientation() * rx.rotX(dTheta) * t;
			mView3D = cam[camIndex].OP.invert();
		}
	}
	else {
		static Matrix t2;
		if (key[KEY_A]) {
			dPsi = 0.001f;
			updateAll = true;
		}
		if (key[KEY_D]) {
			dPsi = -0.001f;
			updateAll = true;
		}
		if (key[KEY_W]) {
			dTheta = -0.001f;
			updateAll = true;
		}
		if (key[KEY_S]) {
			dTheta = 0.001f;
			updateAll = true;
		}
		if (key[KEY_X]) {
			cam[0].elevation -= deltaT;
			updateAll = true;
		}
		if (key[KEY_Y]) {
			cam[0].elevation += deltaT;
			updateAll = true;
		}
		if (key[KEY_LEFT] && !key[KEY_SHIFT] && !key[KEY_CONTROL]) {
			dPhi = 0.01f;
			updateAll = true;
		}
		if (key[KEY_RIGHT] && !key[KEY_SHIFT] && !key[KEY_CONTROL]) {
			dPhi = -0.01f;
			updateAll = true;
		}
		if (key[KEY_UP] && !key[KEY_SHIFT] && !key[KEY_CONTROL]) {
			cam[0].Xi -= 0.01f;
			updateAll = true;
		}
		if (key[KEY_DOWN] && !key[KEY_SHIFT] && !key[KEY_CONTROL]) {
			cam[0].Xi += 0.01f;
			updateAll = true;
		}
		if (updateAll) {
			t[3][1] = cam[0].elevation;
			cam[camIndex].OP = cam[camIndex].OP.orientation() * rz.rotZ(dPsi) * ry.rotY(dPhi) * rx.rotX(dTheta) * t;
			mView3D = (cam[camIndex].OP * rx2.rotX(cam[0].Xi)).invert();
		}
	}

	square->motion();
	filledCircle->motion();
	star->motion();
	perlinCircle->motion();
	welle->motion();
	if (key[KEY_SPACE]) createCommandBuffers();
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