#ifndef VULKAN_DEBUG_H
#define VULKAN_DEBUG_H

#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

VkResult CreateDebugUtilsMessengerEXT(VkInstance vkInstance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(VkInstance vkInstance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

void debugLayers(const char *validationLayers[], int numLayers, GLFWwindow *window, VkInstanceCreateInfo *instanceInfo);

VkResult addDebugMessenger(VkInstance vkInstance, VkDebugUtilsMessengerEXT *debugMessenger, VkAllocationCallbacks *pAllocator);
#endif
