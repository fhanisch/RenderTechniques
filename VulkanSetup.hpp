#pragma once

#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <vulkan/vulkan.h>
#include "Window.hpp"

#define GET_FCN_PTR(fun) \
fun = (PFN_##fun)GetProcAddress(vulkanLib, #fun); \
if (!fun) { \
    std::cout << "Find Symbol '" << #fun << "' failed!\n"; \
    exit(1); \
}

#define GET_INSTANCE_FCN_PTR(fun) \
fun = (PFN_##fun)vkGetInstanceProcAddr(instance, #fun); \
if (!fun) { \
    std::cout << "Find Symbol '" << #fun << "' failed!" << std::endl; \
    exit(1); \
}

#define GET_DEVICE_FCN_PTR(fun) \
fun = (PFN_##fun)vkGetDeviceProcAddr(device, #fun); \
if (!fun) { \
    std::cout << "Find Symbol '" << #fun << "' failed!" << std::endl; \
    exit(1); \
}

class App;
class Image;

class VulkanSetup {
	std::string appName;
	std::string engineName;
	std::string libName;
	HMODULE vulkanLib;
	uint32_t applicationVersion;
	uint32_t engineVersion;
	uint32_t apiVersion;
	uint32_t instanceVersion;
	bool isPreInitialized;
	Window* window;	
	std::vector<const char*> validationLayers;	
	std::vector<const char*> globalExtensions;
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;		
		std::vector<VkSurfaceFormatKHR> formats;		
		std::vector<VkPresentModeKHR> presentModes;
	};
	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo;
	VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkDebugReportCallbackEXT debugReport;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	VkQueue queue;
	VkSwapchainKHR swapChain;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	VkCommandPool commandPool;
	std::vector<Image*> swapChainImages;
	VkRenderPass renderPass;
	VkDescriptorSetLayout descriptorSetLayout;
	Image* depthImage;
	std::vector<VkFramebuffer> swapChainFrameBuffers;
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;

	int preInit();
	void getInstanceVersion();
	void checkVersion();
	void createInstance();
	void setupDebugMessenger();
	void setupDebugReport();
	void createSurface();
	void choosePhysicalDevice();
	void createLogicalDevice();	
	void createSwapChain();
	void createCommandPool();
	void createSwapChainImages();
	void createRenderPass();
	void createDescriptorSetLayout();
	void createDepthImage();
	void createFrameBuffers();
	void createSemaphores();

	/* Helper Functions */
	uint32_t getVersionFromString(const char* str);
	VkDebugUtilsMessengerCreateInfoEXT getDebugMessengerCreateInfo();
	VkDebugReportCallbackCreateInfoEXT getDebugReportCreateInfo();
	bool isDeviceSuitable(VkPhysicalDevice physDevice);
	int findQueueFamilies(VkPhysicalDevice physDevice);
	bool checkDeviceExtensionSupport(VkPhysicalDevice physDevice);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physDevice);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(uint32_t formatCount, VkSurfaceFormatKHR* availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(uint32_t presentModeCount, VkPresentModeKHR* availablePresentModes);
	VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR* capabilities);
	VkFormat findDepthFormat();
	VkFormat findSupportedFormat(uint32_t candidatesCount, VkFormat* candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

public:
	VulkanSetup(App* app);
	~VulkanSetup();
	int init();
	void printPhysicalDevices();
	void printInstanceVersion();
	void printLayers();
	void printExtensions();

	/* Getter */
	VkDevice getDevice();
	VkPhysicalDevice getPhysicalDevice();
	VkCommandPool getCommandPool();
	VkQueue getQueue();
	VkDescriptorSetLayout* getDescriptorSetLayoutPtr();
	VkExtent2D getSwapChainExtent();
	VkRenderPass getRenderPass();
	uint32_t getSwapChainImagesCount();
	std::vector<VkFramebuffer> getSwapChainFrameBuffers();
	VkSwapchainKHR getSwapChain();
	VkSemaphore getImageAvailableSemaphore();
	VkSemaphore getRenderFinishedSemaphore();
};