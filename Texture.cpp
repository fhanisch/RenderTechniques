#include "Texture.hpp"
#include "VulkanSetup.hpp"
#include "Image.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

extern PFN_vkMapMemory vkMapMemory;
extern PFN_vkUnmapMemory vkUnmapMemory;
extern PFN_vkCreateSampler vkCreateSampler;
extern PFN_vkDestroySampler vkDestroySampler;

Texture::Texture(VulkanSetup* _vkSetup, const char* _filename) {
	vkSetup = _vkSetup;
	filename = _filename;
	textureImage = new Image(vkSetup->getPhysicalDevice(), vkSetup->getDevice(), vkSetup->getCommandPool(), vkSetup->getQueue());
	loadTexture();
	createTextureImage();
	createTextureImageView();
	createTextureSampler();
}

/*Texture::Texture(VulkanSetup* _vulkanSetup, TextOverlay* _textOverlay) {
	vulkanSetup = _vulkanSetup;
	texWidth = _textOverlay->texWidth;
	texHeight = _textOverlay->texHeight;
	imageSize = _textOverlay->tetxtureSize;
	pixels = _textOverlay->pixels;
	imageFormat = VK_FORMAT_R8_UNORM;
	textureImage = new Image(vulkanSetup->getPhysicalDevice(), vulkanSetup->getDevice(), vulkanSetup->getCommandPool(), vulkanSetup->getQueue());
	createTextureImage();
	createTextureImageView();
	createTextureSampler();
}*/

Texture::~Texture() {
	vkDestroySampler(vkSetup->getDevice(), textureSampler, nullptr);
	delete textureImage;
}

void Texture::loadTexture() {
	pixels = stbi_load(filename, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	if (!pixels) {
		std::cout << "Failed to load texture image!\n";
		exit(1);
	}
	imageSize = texWidth * texHeight * 4;
	imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
}

void Texture::createTextureImage() {
	Buffer stagingBuffer = Buffer(vkSetup->getPhysicalDevice(), vkSetup->getDevice(), vkSetup->getCommandPool(), vkSetup->getQueue());

	stagingBuffer.createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(vkSetup->getDevice(), stagingBuffer.getBufferMemory(), 0, imageSize, 0, &data);
	memcpy(data, pixels, imageSize);
	vkUnmapMemory(vkSetup->getDevice(), stagingBuffer.getBufferMemory());

	//stbi_image_free(pixels);

	textureImage->createImage(texWidth, texHeight, imageFormat, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	textureImage->transitionImageLayout(imageFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	textureImage->copyBufferToImage(stagingBuffer.getBuffer(), texWidth, texHeight);
	textureImage->transitionImageLayout(imageFormat, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void Texture::createTextureImageView() {
	textureImage->createImageView(imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
}

void Texture::createTextureSampler() {
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(vkSetup->getDevice(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
		std::cout << "Failed to create texture sampler!\n";
		exit(1);
	}
}

VkImageView Texture::getTextureImageView() { return textureImage->getImageView(); }
VkSampler Texture::getTextureSampler() { return textureSampler; }