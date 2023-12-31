#ifndef VULKAN_INIT_H
#define VULKAN_INIT_H

#include "DebugTools/vulkanDebug.h"
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

VkInstance vulkanInit(GLFWwindow* window, VkDebugUtilsMessengerEXT* debugMessenger);

/*Holds a logical device and the associated queue
* @var: vkDevice: Vulkan logical device, type VkDevice
* @var: vkQueue: Vulkan queue, expects only one
* @todo: Add ability for more queues for the device
*/
typedef struct {
    VkDevice vkDevice;
    VkQueue vkQueue;
}Device;

VkResult deviceInit(VkInstance vkInstance, Device *device, VkPhysicalDevice *physicalDevicePtr);

#endif
