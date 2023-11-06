#ifndef VULKAN_INIT_H
#define VULKAN_INIT_H

VkInstance vulkanInit(GLFWwindow* window, VkDebugUtilsMessengerEXT* debugMessenger);

typedef struct //Likely to need editing as complexity rises
{
    VkDevice vkDevice;
    VkQueue vkQueue;
}Device;

Device deviceInit(VkInstance vkInstance);

#endif