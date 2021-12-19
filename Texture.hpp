#pragma once

#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR

#include <vulkan/vulkan.h>

class VulkanSetup;
class Image;

class Texture {
	VulkanSetup* vkSetup;
	const char* filename;
	Image* textureImage;
	int texWidth, texHeight, texChannels;
	VkDeviceSize imageSize;
	unsigned char* pixels;
	VkFormat imageFormat;
	VkSampler textureSampler;
	void loadTexture();
	void createTextureImage();
	void createTextureImageView();
	void createTextureSampler();
public:
	Texture(VulkanSetup* _vkSetup, const char* _filename);
	//Texture(VulkanSetup* _vulkanSetup, TextOverlay* _textOverlay);
	~Texture();
	VkImageView getTextureImageView();
	VkSampler getTextureSampler();
};