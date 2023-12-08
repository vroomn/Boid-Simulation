#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include <stdint.h>
#include <vulkan/vulkan_core.h>

typedef struct {
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR *formats;
    VkPresentModeKHR *presentModes;
} VkSwapChainSupportDetails;

//Wrapper for the VKSwapChainSupportDetails, includes variables to specify string length
typedef struct {
    VkSwapChainSupportDetails vkSwapchain;
    uint32_t formatCount;
    uint32_t presentModeCount;
} SwapChainSupportDetails;

typedef struct {
    VkImage *images;
    uint32_t numImages;
    VkFormat format;
    VkExtent2D extent;
} SwapchainImages;

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

VkSurfaceFormatKHR chooseSwapSurfaceFormat(VkSurfaceFormatKHR *availibleFormats, uint32_t formatCount);

VkPresentModeKHR chooseSwapPresentMode(VkPresentModeKHR *availibleModes, uint32_t modeCount);

VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR capabilities);

VkResult createSwapChain(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, VkSwapchainKHR *swapChain, SwapchainImages *swapChainImages); 

#endif
