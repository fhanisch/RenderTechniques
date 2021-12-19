#pragma once

#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR

#include <vulkan/vulkan.h>

class Buffer {
	size_t size;
	VkBuffer buffer;
	VkDeviceMemory bufferMemory;
	VkCommandBuffer commandBuffer;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkCommandPool commandPool;
	VkQueue queue;
public:
	Buffer(VkPhysicalDevice _physicalDevice, VkDevice _device, VkCommandPool _commandPool, VkQueue _queue);
	~Buffer();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void copyBuffer(VkBuffer srcBuffer, VkDeviceSize size);
	void createBuffer(VkDeviceSize _size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	void createDeviceLocalBuffer(const void* srcData, VkDeviceSize _size, VkBufferUsageFlags bufferUsageFlags);
	void beginSingleTimeCommands();
	void endSingleTimeCommands();
	VkBuffer getBuffer();
	VkCommandBuffer getCommandBuffer();
	VkDeviceMemory getBufferMemory();
};