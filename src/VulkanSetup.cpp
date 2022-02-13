#include "VulkanSetup.hpp"
#include "App.hpp"
#include "Image.hpp"

PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
PFN_vkCreateInstance vkCreateInstance;
PFN_vkDestroyInstance vkDestroyInstance;
PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
PFN_vkCreateDevice vkCreateDevice;
PFN_vkDestroyDevice vkDestroyDevice;
PFN_vkGetDeviceQueue vkGetDeviceQueue;
PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
PFN_vkCreateCommandPool vkCreateCommandPool;
PFN_vkDestroyCommandPool vkDestroyCommandPool;
PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
PFN_vkCreateImageView vkCreateImageView;
PFN_vkDestroyImageView vkDestroyImageView;
PFN_vkCreateRenderPass vkCreateRenderPass;
PFN_vkDestroyRenderPass vkDestroyRenderPass;
PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
PFN_vkCreateImage vkCreateImage;
PFN_vkDestroyImage vkDestroyImage;
PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
PFN_vkAllocateMemory vkAllocateMemory;
PFN_vkFreeMemory vkFreeMemory;
PFN_vkBindImageMemory vkBindImageMemory;
PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
PFN_vkEndCommandBuffer vkEndCommandBuffer;
PFN_vkQueueSubmit vkQueueSubmit;
PFN_vkQueueWaitIdle vkQueueWaitIdle;
PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
PFN_vkCreateFramebuffer vkCreateFramebuffer;
PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
PFN_vkCreateSemaphore vkCreateSemaphore;
PFN_vkDestroySemaphore vkDestroySemaphore;
PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
PFN_vkCreateBuffer vkCreateBuffer;
PFN_vkDestroyBuffer vkDestroyBuffer;
PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
PFN_vkBindBufferMemory vkBindBufferMemory;
PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
PFN_vkCreateShaderModule vkCreateShaderModule;
PFN_vkDestroyShaderModule vkDestroyShaderModule;
PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
PFN_vkDestroyPipeline vkDestroyPipeline;
PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
PFN_vkMapMemory vkMapMemory;
PFN_vkUnmapMemory vkUnmapMemory;
PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
PFN_vkCmdPushConstants vkCmdPushConstants;
PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
PFN_vkCmdBindPipeline vkCmdBindPipeline;
PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
PFN_vkCmdDraw vkCmdDraw;
PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
PFN_vkCreateSampler vkCreateSampler;
PFN_vkDestroySampler vkDestroySampler;
PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
PFN_vkQueuePresentKHR vkQueuePresentKHR;

static VkBool32 debugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	std::cout << "ValidationLayer: " << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}

static VkBool32 debugReportCallback(VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objectType,
	uint64_t object,
	size_t location,
	int32_t messageCode,
	const char* pLayerPrefix,
	const char* pMessage,
	void* pUserData) {

	std::cout << "ValidationLayer: " << pMessage << std::endl;
	return VK_FALSE;
}

VulkanSetup::VulkanSetup(App* app) {
	appName = app->getAppName();
	engineName = app->getEngineName();
	libName = app->getLibName();
	vulkanLib = LoadLibrary(libName.c_str());	
	applicationVersion = getVersionFromString(app->getApplicationVersion().c_str());
	engineVersion = getVersionFromString(app->getEngineVersion().c_str());
	apiVersion = getVersionFromString(app->getApiVersion().c_str());
	isPreInitialized = false;
	window = app->getWindow();
	validationLayers = { "VK_LAYER_KHRONOS_validation" };
	globalExtensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME, VK_EXT_DEBUG_REPORT_EXTENSION_NAME };
	debugUtilsMessengerCreateInfo = getDebugMessengerCreateInfo();
	debugReportCallbackCreateInfo = getDebugReportCreateInfo();
	if (!vulkanLib) {
		std::cout << "Could not load '" << libName << "'!\n";
		exit(1);
	}
	std::cout << "'" << libName << "' loaded.\n";
	GET_FCN_PTR(vkEnumerateInstanceVersion)	
	GET_FCN_PTR(vkEnumerateInstanceLayerProperties)
	GET_FCN_PTR(vkEnumerateInstanceExtensionProperties)
	GET_FCN_PTR(vkGetInstanceProcAddr)
	GET_FCN_PTR(vkCreateInstance)	
	GET_FCN_PTR(vkEnumerateDeviceExtensionProperties)
	GET_FCN_PTR(vkGetPhysicalDeviceProperties)
	GET_FCN_PTR(vkGetPhysicalDeviceFormatProperties)
	GET_FCN_PTR(vkGetPhysicalDeviceFeatures)
	GET_FCN_PTR(vkGetPhysicalDeviceQueueFamilyProperties)
	GET_FCN_PTR(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
	GET_FCN_PTR(vkGetPhysicalDeviceSurfaceFormatsKHR)
	GET_FCN_PTR(vkGetPhysicalDeviceSurfacePresentModesKHR)
	GET_FCN_PTR(vkGetPhysicalDeviceSurfaceSupportKHR)
	GET_FCN_PTR(vkGetPhysicalDeviceMemoryProperties)
	GET_FCN_PTR(vkCreateDevice)
	GET_FCN_PTR(vkGetDeviceQueue)
	GET_FCN_PTR(vkBeginCommandBuffer)
	GET_FCN_PTR(vkEndCommandBuffer)
	GET_FCN_PTR(vkQueueSubmit)
	GET_FCN_PTR(vkQueueWaitIdle)
	GET_FCN_PTR(vkCmdPipelineBarrier)
	GET_FCN_PTR(vkCmdBeginRenderPass)
	GET_FCN_PTR(vkCmdEndRenderPass)
	GET_FCN_PTR(vkCmdPushConstants)
	GET_FCN_PTR(vkCmdBindIndexBuffer)
	GET_FCN_PTR(vkCmdBindVertexBuffers)
	GET_FCN_PTR(vkCmdBindPipeline)
	GET_FCN_PTR(vkCmdBindDescriptorSets)
	GET_FCN_PTR(vkCmdDraw)
	GET_FCN_PTR(vkCmdDrawIndexed)
	GET_FCN_PTR(vkCmdCopyBufferToImage)
	GET_FCN_PTR(vkQueuePresentKHR)
}

VulkanSetup::~VulkanSetup() {
	std::cout << "Clean up Vulkan." << std::endl;
	vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
	vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
	for (uint32_t i = 0; i < swapChainFrameBuffers.size(); i++) {
		vkDestroyFramebuffer(device, swapChainFrameBuffers[i], nullptr);
	}
	delete depthImage;
	vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);
	for (uint32_t i = 0; i < swapChainImages.size(); i++) {
		delete swapChainImages[i];
	}
	vkDestroyCommandPool(device, commandPool, nullptr);
	vkDestroySwapchainKHR(device, swapChain, nullptr);
	vkDestroyDevice(device, nullptr);
	//vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyDebugReportCallbackEXT(instance, debugReport, nullptr);
	vkDestroyInstance(instance, nullptr);
}

int VulkanSetup::preInit() {
	getInstanceVersion();
	//checkVersion(); //TODO: Check instanceVerion == apiVersion?
	createInstance();
	//GET_INSTANCE_FCN_PTR(vkCreateDebugUtilsMessengerEXT)
	//GET_INSTANCE_FCN_PTR(vkDestroyDebugUtilsMessengerEXT)
	GET_INSTANCE_FCN_PTR(vkDestroyInstance)
	GET_INSTANCE_FCN_PTR(vkCreateDebugReportCallbackEXT)
	GET_INSTANCE_FCN_PTR(vkDestroyDebugReportCallbackEXT)
	GET_INSTANCE_FCN_PTR(vkCreateWin32SurfaceKHR)
	GET_INSTANCE_FCN_PTR(vkDestroySurfaceKHR)
	GET_INSTANCE_FCN_PTR(vkEnumeratePhysicalDevices)
	GET_INSTANCE_FCN_PTR(vkGetDeviceProcAddr)
	//setupDebugMessenger();
	setupDebugReport();
	createSurface();
	isPreInitialized = true;
	return 0;
}

int VulkanSetup::init() {
	if (!isPreInitialized) preInit();	
	choosePhysicalDevice();
	createLogicalDevice();
	GET_DEVICE_FCN_PTR(vkDestroyDevice)
	GET_DEVICE_FCN_PTR(vkCreateSwapchainKHR)
	GET_DEVICE_FCN_PTR(vkDestroySwapchainKHR)
	GET_DEVICE_FCN_PTR(vkCreateCommandPool)
	GET_DEVICE_FCN_PTR(vkDestroyCommandPool)
	GET_DEVICE_FCN_PTR(vkGetSwapchainImagesKHR)
	GET_DEVICE_FCN_PTR(vkCreateImageView)
	GET_DEVICE_FCN_PTR(vkDestroyImageView)
	GET_DEVICE_FCN_PTR(vkCreateRenderPass)
	GET_DEVICE_FCN_PTR(vkDestroyRenderPass)
	GET_DEVICE_FCN_PTR(vkCreateDescriptorSetLayout)
	GET_DEVICE_FCN_PTR(vkDestroyDescriptorSetLayout)
	GET_DEVICE_FCN_PTR(vkCreateImage)
	GET_DEVICE_FCN_PTR(vkDestroyImage)
	GET_DEVICE_FCN_PTR(vkGetImageMemoryRequirements)
	GET_DEVICE_FCN_PTR(vkAllocateMemory)
	GET_DEVICE_FCN_PTR(vkFreeMemory)
	GET_DEVICE_FCN_PTR(vkBindImageMemory)
	GET_DEVICE_FCN_PTR(vkAllocateCommandBuffers)
	GET_DEVICE_FCN_PTR(vkFreeCommandBuffers)
	GET_DEVICE_FCN_PTR(vkCreateFramebuffer)
	GET_DEVICE_FCN_PTR(vkDestroyFramebuffer)
	GET_DEVICE_FCN_PTR(vkCreateSemaphore)
	GET_DEVICE_FCN_PTR(vkDestroySemaphore)
	GET_DEVICE_FCN_PTR(vkCreateDescriptorPool)
	GET_DEVICE_FCN_PTR(vkDestroyDescriptorPool)
	GET_DEVICE_FCN_PTR(vkCreateBuffer)
	GET_DEVICE_FCN_PTR(vkDestroyBuffer)
	GET_DEVICE_FCN_PTR(vkGetBufferMemoryRequirements)
	GET_DEVICE_FCN_PTR(vkBindBufferMemory)
	GET_DEVICE_FCN_PTR(vkCreatePipelineLayout)
	GET_DEVICE_FCN_PTR(vkDestroyPipelineLayout)
	GET_DEVICE_FCN_PTR(vkCreateShaderModule)
	GET_DEVICE_FCN_PTR(vkDestroyShaderModule)
	GET_DEVICE_FCN_PTR(vkCreateGraphicsPipelines)
	GET_DEVICE_FCN_PTR(vkDestroyPipeline)
	GET_DEVICE_FCN_PTR(vkAllocateDescriptorSets)
	GET_DEVICE_FCN_PTR(vkFreeDescriptorSets)
	GET_DEVICE_FCN_PTR(vkUpdateDescriptorSets)
	GET_DEVICE_FCN_PTR(vkMapMemory)
	GET_DEVICE_FCN_PTR(vkUnmapMemory)
	GET_DEVICE_FCN_PTR(vkCmdCopyBuffer)
	GET_DEVICE_FCN_PTR(vkCreateSampler)
	GET_DEVICE_FCN_PTR(vkDestroySampler)
	GET_DEVICE_FCN_PTR(vkAcquireNextImageKHR)
	createSwapChain();
	createCommandPool();
	createSwapChainImages();
	createRenderPass();
	createDescriptorSetLayout();
	createDepthImage();
	createFrameBuffers();
	createSemaphores();
	return 0;
}

void VulkanSetup::printPhysicalDevices() {
	if (!isPreInitialized) preInit();
	uint32_t deviceCount;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if (deviceCount == 0) {
		std::cout << "Failed to find GPUs with Vulkan support!\n" << std::endl;
		return;
	}
	VkPhysicalDevice* devices = new VkPhysicalDevice[deviceCount];
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

	std::cout << "Physical devices found: " << deviceCount << std::endl;

	for (uint32_t i = 0; i < deviceCount; i++) {
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);
		std::cout << "Device #" << i << "(" << deviceProperties.deviceName << ")" << ": \n";
		std::cout << "\tDevice Properties:\n";
		std::cout << "\t\tApi Version:    " << deviceProperties.apiVersion << std::endl;
		std::cout << "\t\tDevice ID:      " << deviceProperties.deviceID << std::endl;
		std::cout << "\t\tDevice Name:    " << deviceProperties.deviceName << std::endl;
		std::cout << "\t\tDevice Type:    " << deviceProperties.deviceType << std::endl;
		std::cout << "\t\tDriver Version: " << deviceProperties.driverVersion << std::endl;
		std::cout << "\t\tVendor ID:      " << deviceProperties.vendorID << std::endl;

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(devices[i], &deviceFeatures);
		std::cout << "\tDevice Featers:\n";
		std::cout << "\t\tTessellation Shader: " << deviceFeatures.tessellationShader << std::endl;
		std::cout << "\t\tGeometry Shader:     " << deviceFeatures.geometryShader << std::endl;
		std::cout << "\t\tfullDrawIndexUint32: " << deviceFeatures.fullDrawIndexUint32 << std::endl;
		std::cout << "\t\tsamplerAnisotropy:   " << deviceFeatures.samplerAnisotropy << std::endl;
		std::cout << "\t\twideLines:           " << deviceFeatures.wideLines << std::endl;

		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(devices[i], nullptr, &extensionCount, nullptr);
		VkExtensionProperties* extensions = new VkExtensionProperties[extensionCount];
		vkEnumerateDeviceExtensionProperties(devices[i], nullptr, &extensionCount, extensions);
		std::cout << "\tNumber of device extensions: " << extensionCount << std::endl;
		for (uint32_t j = 0; j < extensionCount; j++) {
			std::cout << "\t" << std::setw(9) << j << ": " << extensions[j].extensionName << std::endl;
		}
		delete[] extensions;

		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, nullptr);
		VkQueueFamilyProperties* queueFamilies = new VkQueueFamilyProperties[queueFamilyCount];
		vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, queueFamilies);
		std::cout << "\tNumber of device queue families: " << queueFamilyCount << std::endl;
		for (uint32_t j = 0; j < queueFamilyCount; j++) {
			std::cout << "\t\tQueue Family: " << j << std::endl;
			std::cout << "\t\t\tVK_QUEUE_GRAPHICS_BIT:       " << ((queueFamilies[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) << std::endl;
			std::cout << "\t\t\tVK_QUEUE_COMPUTE_BIT:        " << ((queueFamilies[j].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0) << std::endl;
			std::cout << "\t\t\tVK_QUEUE_TRANSFER_BIT:       " << ((queueFamilies[j].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0) << std::endl;
			std::cout << "\t\t\tVK_QUEUE_SPARSE_BINDING_BIT: " << ((queueFamilies[j].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) != 0) << std::endl;
			std::cout << "\t\t\tQueue Count:                 " << queueFamilies[j].queueCount << std::endl;
			std::cout << "\t\t\tTimestamp Valid Bits:        " << queueFamilies[j].timestampValidBits << std::endl;
			uint32_t width = queueFamilies[j].minImageTransferGranularity.width;
			uint32_t height = queueFamilies[j].minImageTransferGranularity.height;
			uint32_t depth = queueFamilies[j].minImageTransferGranularity.depth;
			std::cout << "\t\t\tMin Image Timestamp Granularity (width, height, depth): " << width << ", " << height << ", " << depth << std::endl;
		}
		delete[] queueFamilies;

		VkSurfaceCapabilitiesKHR capabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(devices[i], surface, &capabilities);
		std::cout << "\tDevice Surface capabilities:\n";
		std::cout << "\t\tminImageCount:           " << capabilities.minImageCount << std::endl;
		std::cout << "\t\tmaxImageCount:           " << capabilities.maxImageCount << std::endl;
		std::cout << "\t\tcurrentExtent:           " << capabilities.currentExtent.width << ", " << capabilities.currentExtent.height << std::endl;
		std::cout << "\t\tminImageExtent:          " << capabilities.minImageExtent.width << ", " << capabilities.minImageExtent.height << std::endl;
		std::cout << "\t\tmaxImageExtent:          " << capabilities.maxImageExtent.width << ", " << capabilities.maxImageExtent.height << std::endl;
		std::cout << "\t\tmaxImageArrayLayers:     " << capabilities.maxImageArrayLayers << std::endl;
		std::cout << "\t\tsupportedTransforms:     " << capabilities.supportedTransforms << std::endl;
		std::cout << "\t\tcurrentTransform:        " << capabilities.currentTransform << std::endl;
		std::cout << "\t\tsupportedCompositeAlpha: " << capabilities.supportedCompositeAlpha << std::endl;
		std::cout << "\t\tsupportedUsageFlags:     " << capabilities.supportedUsageFlags << std::endl;

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(devices[i], surface, &formatCount, nullptr);
		VkSurfaceFormatKHR* formats = new VkSurfaceFormatKHR[formatCount];
		vkGetPhysicalDeviceSurfaceFormatsKHR(devices[i], surface, &formatCount, formats);
		std::cout << "\tNumber of device supported formats: " << formatCount << std::endl;
		for (uint32_t j = 0; j < formatCount; j++) {
			std::cout << "\t\tFormat: " << j << std::endl;
			std::cout << "\t\t\tformat:     " << formats[j].format << std::endl;
			std::cout << "\t\t\tcolorSpace: " << formats[j].colorSpace << std::endl;
		}
		delete[] formats;

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(devices[i], surface, &presentModeCount, nullptr);
		VkPresentModeKHR* presentModes = new VkPresentModeKHR[presentModeCount];
		vkGetPhysicalDeviceSurfacePresentModesKHR(devices[i], surface, &presentModeCount, presentModes);
		std::cout << "\tNumber of device supported presentation modes: " << presentModeCount << std::endl;
		for (uint32_t j = 0; j < presentModeCount; j++) {
			std::cout << "\t\tPresent mode #" << j << ": " << presentModes[j] << std::endl;
		}
		delete[] presentModes;

		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(devices[i], &memProperties);
		std::cout << "\tNumber of memoryTypes: " << memProperties.memoryTypeCount << std::endl;
		for (uint32_t j = 0; j < memProperties.memoryTypeCount; j++) {
			std::cout << "\t\tMemory Type: " << j << std::endl;
			std::cout << "\t\t\tVK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT: " << ((memProperties.memoryTypes[j].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0) << std::endl;
			std::cout << "\t\t\tVK_MEMORY_PROPERTY_HOST_VISIBLE_BIT: " << ((memProperties.memoryTypes[j].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0) << std::endl;
			std::cout << "\t\t\tVK_MEMORY_PROPERTY_HOST_COHERENT_BIT: " << ((memProperties.memoryTypes[j].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != 0) << std::endl;
		}
	}

	delete[] devices;
}

void VulkanSetup::getInstanceVersion() {
	vkEnumerateInstanceVersion(&instanceVersion);
	std::cout << "Supported Api Version: " << VK_API_VERSION_VARIANT(instanceVersion) << "." << VK_API_VERSION_MAJOR(instanceVersion) << "." << VK_API_VERSION_MINOR(instanceVersion) << "." << VK_API_VERSION_PATCH(instanceVersion) << std::endl;
	std::cout << std::endl;
}

void VulkanSetup::checkVersion() {
	//TODO: Check instanceVerion == apiVersion?
}

void VulkanSetup::printInstanceVersion() {
	getInstanceVersion();
}

void VulkanSetup::printLayers() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	VkLayerProperties* availableLayers = new VkLayerProperties[layerCount];
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);
	std::cout << "Available Intance Layers: " << layerCount << std::endl;
	for (uint32_t i = 0; i < layerCount; i++) {
		std::cout << std::setw(5) << i << ": " << std::setiosflags(std::ios::left) << std::setw(50) << availableLayers[i].layerName
			<< std::setw(50) << availableLayers[i].description << std::resetiosflags(std::ios::left)
			<< std::endl;
	}
	std::cout << std::endl;
	delete[] availableLayers;
}

void VulkanSetup::printExtensions() {
	uint32_t extensionCount;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	VkExtensionProperties* extensions = new VkExtensionProperties[extensionCount];
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions);
	std::cout << "Available Intance Extensions: " << extensionCount << std::endl;
	for (uint32_t i = 0; i < extensionCount; i++) {
		std::cout << std::setw(5) << i << ": " << extensions[i].extensionName << std::endl;
	}
	std::cout << std::endl;
	delete[] extensions;
}

void VulkanSetup::createInstance() {
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = NULL;
	appInfo.pApplicationName = appName.c_str();
	appInfo.applicationVersion = applicationVersion;
	appInfo.pEngineName = engineName.c_str();
	appInfo.engineVersion = engineVersion;
	appInfo.apiVersion = apiVersion;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pNext = nullptr;// für Zusazinformationen: createInfo.pNext = &debugReportCallbackCreateInfo;
	createInfo.flags = 0;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledLayerCount = (uint32_t)validationLayers.size();
	createInfo.ppEnabledLayerNames = validationLayers.data();
	createInfo.enabledExtensionCount = (uint32_t)globalExtensions.size();
	createInfo.ppEnabledExtensionNames = globalExtensions.data();

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		std::cout << "Failed to create instance!" << std::endl;
		exit(1);
	}
	std::cout << "Instance created." << std::endl;
}

void VulkanSetup::setupDebugMessenger() {
	if (vkCreateDebugUtilsMessengerEXT(instance, &debugUtilsMessengerCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		std::cout << "Failed to set up debug messenger!" << std::endl;
		exit(1);
	}
	std::cout << "Debug Messenger created." << std::endl;
}

void VulkanSetup::setupDebugReport() {
	if (vkCreateDebugReportCallbackEXT(instance, &debugReportCallbackCreateInfo, nullptr, &debugReport) != VK_SUCCESS) {
		std::cout << "Failed to set up debug report!" << std::endl;
		exit(1);
	}
	std::cout << "Debug Report created." << std::endl;
}

void VulkanSetup::createSurface() {
	VkWin32SurfaceCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = window->getWindow();
	createInfo.hinstance = window->getInstance();

	if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
		std::cout << "Failed to create window surface!" << std::endl;
		exit(1);
	}
	std::cout << "Surface created." << std::endl;
}

void VulkanSetup::choosePhysicalDevice() {
	uint32_t deviceCount;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if (deviceCount == 0) {
		std::cout << "Failed to find GPUs with Vulkan support!\n";
		exit(1);
	}
	VkPhysicalDevice* devices = new VkPhysicalDevice[deviceCount];
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices);
	for (unsigned int i = 0; i < deviceCount; i++) {
		if (isDeviceSuitable(devices[i])) {
			physicalDevice = devices[i];			
			break;
		}
	}
	if (physicalDevice == VK_NULL_HANDLE) {
		std::cout << "Failed to find a suitable GPU!\n";
		exit(1);
	}
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
	std::cout << "Physical Device '" << deviceProperties.deviceName << "' selected.\n";
	delete[] devices;
}

void VulkanSetup::createLogicalDevice() {
	uint32_t queueFamilyIndex = findQueueFamilies(physicalDevice);
	float queuePriority = 1.0f;

	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.pNext = NULL;
	queueCreateInfo.flags = 0;
	queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.tessellationShader = VK_TRUE;
	deviceFeatures.samplerAnisotropy = VK_TRUE;
	deviceFeatures.wideLines = VK_TRUE;
	
	std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pNext = NULL;
	createInfo.flags = 0;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.enabledLayerCount = 0;
	createInfo.ppEnabledLayerNames = nullptr; // evtl. auch hier den validationLayer eintragen
	createInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	createInfo.pEnabledFeatures = &deviceFeatures;

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
		std::cout << "Failed to create logical device!\n";
		exit(1);
	}
	std::cout << "Device created.\n";
	vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);
}

void VulkanSetup::createSwapChain() {
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice); // --> ToDo: delete des Inhalts beim Beenden 
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat((uint32_t)swapChainSupport.formats.size(), swapChainSupport.formats.data());
	VkPresentModeKHR presentMode = chooseSwapPresentMode((uint32_t)swapChainSupport.presentModes.size(), swapChainSupport.presentModes.data());
	VkExtent2D extent = chooseSwapExtent(&swapChainSupport.capabilities);
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.queueFamilyIndexCount = 0;
	createInfo.pQueueFamilyIndices = nullptr;
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
		std::cout << "Failed to create swap chain!\n";
		exit(1);
	}
	std::cout << "imageCount: " << imageCount << std::endl;
	std::cout << "Swapchain created.\n";
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;	
}

void VulkanSetup::createCommandPool() {
	int queueFamilyIndex = findQueueFamilies(physicalDevice);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = 0; // Optional
	poolInfo.queueFamilyIndex = queueFamilyIndex;

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		std::cout << "failed to create command pool!\n";
		exit(1);
	}
	std::cout << "Command Pool created.\n";
}

void VulkanSetup::createSwapChainImages() {
	uint32_t swapChainImagesCount;
	vkGetSwapchainImagesKHR(device, swapChain, &swapChainImagesCount, nullptr);
	VkImage* tmp = new VkImage[swapChainImagesCount];	
	vkGetSwapchainImagesKHR(device, swapChain, &swapChainImagesCount, tmp);

	for (uint32_t i = 0; i < swapChainImagesCount; i++) {		
		swapChainImages.push_back(new Image(physicalDevice, device, commandPool, queue));
		swapChainImages[i]->setImage(tmp[i], true);
		swapChainImages[i]->createImageView(swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}
	std::cout << swapChainImagesCount << " image views created." << std::endl;
	delete[] tmp;
}

void VulkanSetup::createRenderPass() {
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.flags = 0;
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = VK_FORMAT_D32_SFLOAT;
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.flags = 0;
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkAttachmentDescription attachments[] = { colorAttachment, depthAttachment };

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 2;
	renderPassInfo.pAttachments = attachments;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
		std::cout << "Failed to create render pass!\n";
		exit(1);
	}
	std::cout << "Renderpass created.\n";
}

void VulkanSetup::createDescriptorSetLayout() {
	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
	uboLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	samplerLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding uboMaterialBinding = {};
	uboMaterialBinding.binding = 2;
	uboMaterialBinding.descriptorCount = 1;
	uboMaterialBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboMaterialBinding.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
	uboMaterialBinding.pImmutableSamplers = nullptr;

	std::vector<VkDescriptorSetLayoutBinding> bindings = { uboLayoutBinding, samplerLayoutBinding, uboMaterialBinding};

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = (uint32_t)bindings.size();
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		std::cout << "Failed to create descriptor set layout!\n";
		exit(1);
	}
	std::cout << "DescriptorSetLayout created.\n";
}

void VulkanSetup::createDepthImage() {
	VkFormat depthFormat = findDepthFormat();
	depthImage = new Image(physicalDevice, device, commandPool, queue);
	depthImage->createImage(swapChainExtent.width, swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	depthImage->createImageView(depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
	depthImage->transitionImageLayout(depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	std::cout << "Depth Image created.\n";
}

void VulkanSetup::createFrameBuffers() {
	swapChainFrameBuffers = std::vector<VkFramebuffer>(swapChainImages.size());

	for (uint32_t i = 0; i < swapChainFrameBuffers.size(); i++) {
		VkImageView attachments[] = { swapChainImages[i]->getImageView(), depthImage->getImageView() };
		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 2;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFrameBuffers[i]) != VK_SUCCESS) {
			std::cout << "failed to create framebuffer!\n";
			exit(1);
		}
	}
	std::cout << "Framebuffers created.\n";
}

void VulkanSetup::createSemaphores() {
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
		vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS) {

		std::cout << "failed to create semaphores!\n";
		exit(1);
	}
	std::cout << "Semaphores created.\n";
}

/* Helper Functions */

uint32_t VulkanSetup::getVersionFromString(const char* str) {	
	int i = 0;
	int j = 0;
	uint32_t v[4];
	while (str[i]) {
		if (str[i] == '.') {			
			sscanf(str, "%u", &v[j]);
			str += i + 1;
			i = 0;
			j++;
		}
		else {
			i++;
		}
	}
	sscanf(str, "%u", &v[j]);	
	return VK_MAKE_API_VERSION(v[0], v[1], v[2], v[3]);
}

VkDebugUtilsMessengerCreateInfoEXT VulkanSetup::getDebugMessengerCreateInfo() {
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugMessengerCallback;
	return createInfo;
}

VkDebugReportCallbackCreateInfoEXT VulkanSetup::getDebugReportCreateInfo() {
	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.pfnCallback = debugReportCallback;
	createInfo.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT;// | VK_DEBUG_REPORT_DEBUG_BIT_EXT;
	return createInfo;
}

bool VulkanSetup::isDeviceSuitable(VkPhysicalDevice physDevice) {
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(physDevice, &deviceProperties);

	int index = findQueueFamilies(physDevice);
	bool extensionsSupported = checkDeviceExtensionSupport(physDevice);
	bool swapChainAdequate = false;

	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physDevice);
		swapChainAdequate = swapChainSupport.formats.size() && swapChainSupport.presentModes.size();
	}

	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDevice, surface, &capabilities);

	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && index >= 0 && extensionsSupported && swapChainAdequate && (capabilities.maxImageCount >= 0); // Eigentlich sollte maxImageCount>2 sein. Ist in manchen Fällen aber maxImageCount=0.
}

int VulkanSetup::findQueueFamilies(VkPhysicalDevice physDevice)
{
	int index = -1;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &queueFamilyCount, nullptr);
	VkQueueFamilyProperties* queueFamilies = new VkQueueFamilyProperties[queueFamilyCount];
	vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &queueFamilyCount, queueFamilies);

	for (unsigned int i = 0; i < queueFamilyCount; i++) {
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physDevice, i, surface, &presentSupport);
		if (queueFamilies[i].queueCount > 0 && queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupport) {
			index = i;
		}
		if (index >= 0) break;
	}
	delete[] queueFamilies;
	return index;
}

bool VulkanSetup::checkDeviceExtensionSupport(VkPhysicalDevice physDevice) {
	bool extensionsSupported = false;
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(physDevice, nullptr, &extensionCount, nullptr);
	VkExtensionProperties* availableExtensions = new VkExtensionProperties[extensionCount];
	vkEnumerateDeviceExtensionProperties(physDevice, nullptr, &extensionCount, availableExtensions);

	for (uint32_t i = 0; i < extensionCount; i++) {
		if (std::string(availableExtensions[i].extensionName) == VK_KHR_SWAPCHAIN_EXTENSION_NAME) {
			extensionsSupported = true;
			break;
		}
	}
	delete[] availableExtensions;
	return extensionsSupported;
}

VulkanSetup::SwapChainSupportDetails VulkanSetup::querySwapChainSupport(VkPhysicalDevice physDevice) {
	SwapChainSupportDetails details;
	uint32_t formatCount;
	uint32_t presentModeCount;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDevice, surface, &details.capabilities);

	vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, nullptr);	
	details.formats = std::vector<VkSurfaceFormatKHR>(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, details.formats.data());

	vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presentModeCount, nullptr);	
	details.presentModes = std::vector<VkPresentModeKHR>(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presentModeCount, details.presentModes.data());

	return details;
}

VkSurfaceFormatKHR VulkanSetup::chooseSwapSurfaceFormat(uint32_t formatCount, VkSurfaceFormatKHR* availableFormats) {
	if (formatCount == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}
	for (uint32_t i = 0; i < formatCount; i++) {
		if (availableFormats[i].format == VK_FORMAT_B8G8R8A8_UNORM && availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return availableFormats[i];
	}
	return availableFormats[0];
}

VkPresentModeKHR VulkanSetup::chooseSwapPresentMode(uint32_t presentModeCount, VkPresentModeKHR* availablePresentModes) {
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (uint32_t i = 0; i < presentModeCount; i++) {
		if (availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			return availablePresentModes[i];
		else if (availablePresentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)
			bestMode = availablePresentModes[i];
	}

	return bestMode;
}

#undef max
#undef min
VkExtent2D VulkanSetup::chooseSwapExtent(VkSurfaceCapabilitiesKHR* capabilities) {
	if (capabilities->currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities->currentExtent;
	}
	else {
		VkExtent2D actualExtent = { (uint32_t)(window)->getWidth(), (uint32_t)(window)->getHeight() };
		actualExtent.width = std::max(capabilities->minImageExtent.width, std::min(capabilities->maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities->minImageExtent.height, std::min(capabilities->maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

VkFormat VulkanSetup::findDepthFormat() {
	VkFormat formats[] = { VK_FORMAT_D32_SFLOAT };
	return findSupportedFormat(1, formats, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

VkFormat VulkanSetup::findSupportedFormat(uint32_t candidatesCount, VkFormat* candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
	for (uint32_t i = 0; i < candidatesCount; i++) {
		VkFormatProperties properties;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, candidates[i], &properties);
		if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features) {
			return candidates[i];
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features) {
			return candidates[i];
		}
	}
	std::cout << "Failed to find supported format!\n";
	exit(1);
}

/* Getters */
VkDevice VulkanSetup::getDevice() { return device; }
VkPhysicalDevice VulkanSetup::getPhysicalDevice() { return physicalDevice; }
VkCommandPool VulkanSetup::getCommandPool() { return commandPool; }
VkQueue VulkanSetup::getQueue() { return queue; }
VkDescriptorSetLayout* VulkanSetup::getDescriptorSetLayoutPtr() { return &descriptorSetLayout; }
VkExtent2D VulkanSetup::getSwapChainExtent() { return swapChainExtent; }
VkRenderPass VulkanSetup::getRenderPass() { return renderPass; }
uint32_t VulkanSetup::getSwapChainImagesCount() { return (uint32_t)swapChainImages.size(); }
std::vector<VkFramebuffer> VulkanSetup::getSwapChainFrameBuffers() { return swapChainFrameBuffers; }
VkSwapchainKHR VulkanSetup::getSwapChain() { return swapChain; }
VkSemaphore VulkanSetup::getImageAvailableSemaphore() { return imageAvailableSemaphore; }
VkSemaphore VulkanSetup::getRenderFinishedSemaphore() { return renderFinishedSemaphore; }