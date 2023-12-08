#include "Initalization/imageViews.h"
#include "Initalization/swapchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

VkResult createImageViews(SwapchainImages *swapChainImages, SwapchainImageViews *swapChainImageViews, VkDevice device) {
    swapChainImageViews->numViews = swapChainImages->numImages;
    swapChainImageViews->imageViews = (VkImageView *)malloc(swapChainImageViews->numViews * sizeof(VkImageView));

    VkImageViewCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = swapChainImages->format;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    for (int i = 0; i < swapChainImageViews->numViews; i++) {
        createInfo.image = swapChainImages->images[i];

        if (vkCreateImageView(device, &createInfo, NULL, &swapChainImageViews->imageViews[i]) != VK_SUCCESS) {
            printf("Failed to create image view!\n");
            return VK_ERROR_INITIALIZATION_FAILED;
        }
    }

    return VK_SUCCESS;
}
