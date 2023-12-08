#ifndef IMAGEVIEWS_H
#define IMAGEVIEWS_H

#include <vulkan/vulkan_core.h>
#include "swapchain.h"

typedef struct {
    VkImageView *imageViews;
    uint32_t numViews;
} SwapchainImageViews;

VkResult createImageViews(SwapchainImages *swapChainImages, SwapchainImageViews *swapChainImageViews, VkDevice device); 

#endif
