#pragma once

#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR

#include <vulkan/vulkan.h>
#include "Buffer.hpp"

class Image {
	VkImage image;
	VkImageView imageView;
	VkDeviceMemory imageMemory;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkCommandPool commandPool;
	VkQueue queue;
	Buffer* commandBuffer;
	bool isSwapChainImage = false;
public:
	Image(VkPhysicalDevice _physicalDevice, VkDevice _device, VkCommandPool _commandPool, VkQueue _queue);
	~Image();
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
	void createImageView(VkFormat format, VkImageAspectFlags aspectFlags);
	void transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void copyBufferToImage(VkBuffer buffer, uint32_t width, uint32_t height);
	bool hasStencilComponent(VkFormat format);
	void setImage(VkImage _image, bool _isSwapChainImage);
	VkImageView getImageView();
};