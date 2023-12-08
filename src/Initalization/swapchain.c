#include "Initalization/swapchain.h"
#include <stdint.h>
#include <stdio.h>
#include <vulkan/vulkan_core.h>
#include <stdlib.h>

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SwapChainSupportDetails details;
    
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.vkSwapchain.capabilities);

    //Gets the formats and how many there are
    details.formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &details.formatCount, NULL);
    details.vkSwapchain.formats = (VkSurfaceFormatKHR *)malloc(details.formatCount * sizeof(VkSurfaceFormatKHR));
    if (details.formatCount != 0) {
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &details.formatCount, details.vkSwapchain.formats);
    }

    details.presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &details.presentModeCount, NULL);
    details.vkSwapchain.presentModes = (VkPresentModeKHR *)malloc(details.presentModeCount * sizeof(VkPresentModeKHR));
    if (details.presentModeCount != 0) {
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &details.presentModeCount, details.vkSwapchain.presentModes);
    }

    return details;
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(VkSurfaceFormatKHR *availibleFormats, uint32_t formatCount) {
    //Iterate through the array of availbile formats and selects one that meets the criterion
    for (int i = 0; i < formatCount; i++) {
        if (availibleFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB && availibleFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availibleFormats[i];
        }
    }

    //Default value
    return availibleFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(VkPresentModeKHR *availibleModes, uint32_t modeCount) {
    //Iterate to see if desired display mode is availible
    for (int i = 0; i < modeCount; i++) {
        if (availibleModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            return  availibleModes[i];
        }
    }

    //Default swap mode (double buffer)
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR capabilities) {
    //FIXME: Account for non-standard dpi screen
    return capabilities.currentExtent;
}

VkResult createSwapChain(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, VkSwapchainKHR *swapChain, SwapchainImages *swapChainImages) {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);

    if (swapChainSupport.presentModeCount == 0 || swapChainSupport.presentModeCount == 0) {
        printf("Swapchain not fully supported!\n");
        return VK_INCOMPLETE;
    }

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.vkSwapchain.formats, swapChainSupport.formatCount);
    free(swapChainSupport.vkSwapchain.formats);

    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.vkSwapchain.presentModes, swapChainSupport.presentModeCount);
    free(swapChainSupport.vkSwapchain.presentModes);

    //What is the extent (size) of the swapchain
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.vkSwapchain.capabilities);
    //How many images can be in the swapchain
    uint32_t imageCount = swapChainSupport.vkSwapchain.capabilities.minImageCount + 1;
    if (swapChainSupport.vkSwapchain.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.vkSwapchain.capabilities.maxImageCount) {
        imageCount = swapChainSupport.vkSwapchain.capabilities.maxImageCount;
    }

    //Info to feed into the swapchain generator
    VkSwapchainCreateInfoKHR createInfo = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    //TODO: Learn later how to do exclusive sharing and only use that
    //TODO: Figure out the queue families thing
    uint32_t familyIndicie = -1;
    uint32_t presentFamily = -1;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, NULL);
    VkQueueFamilyProperties queueFamilies[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies);
    for (int i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            familyIndicie = i;
        }

        VkBool32 presentSupport = 0;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
        if (presentSupport) {
            presentFamily = i;        
        }
    }
    if (familyIndicie == -1 || presentFamily == -1) {
        return VK_ERROR_FEATURE_NOT_PRESENT;
    }

    uint32_t queueFamilyIndicies[] = {familyIndicie, presentFamily};
    if (familyIndicie != presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndicies;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.vkSwapchain.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device, &createInfo, NULL, swapChain) != VK_SUCCESS) {
        printf("Failed to generate swapchain\n");
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    //Grab the images within the swapchain
    vkGetSwapchainImagesKHR(device, *swapChain, &swapChainImages->numImages, NULL);
    //Alllocate memory for the list of swapchain images
    swapChainImages->images = (VkImage *)malloc(swapChainImages->numImages * sizeof(VkImage));
    vkGetSwapchainImagesKHR(device, *swapChain, &swapChainImages->numImages, swapChainImages->images); 

    //Assign supplamentary image data
    swapChainImages->format = surfaceFormat.format;
    swapChainImages->extent = extent;

    return VK_SUCCESS;
}
