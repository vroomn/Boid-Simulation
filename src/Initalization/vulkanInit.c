#include <stdint.h>
#include <vulkan/vulkan_core.h>
#include "DebugTools/vulkanDebug.h"
#include "vulkanInit.h"
#include "glfw/glfwFuncs.h"
#include <stdio.h>
#include <string.h>

/*Create a new Vulkan instance, adjust for debug mode
* @param window: GLFW window object to do draw calls on
* @param debugMessenger: A messenger id that can allow the debug system to be destroyed
*/
VkInstance vulkanInit(GLFWwindow* window, VkDebugUtilsMessengerEXT* debugMessenger) {
    //Information to feed into the VkInstance
    VkApplicationInfo appInfo = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Boid Simulation";
    appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0); //Vulkan tutorial refrence used deprecated macro, taken from refrence docs
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    //VkInstance info to initalize
    VkInstanceCreateInfo createInfo = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    //Validation layer config
    //GLFW vulkan extention info
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    #if DEBUG_MODE == 1
        printf("Debug Mode\n");

        const char *validationLayers[] = {
            "VK_LAYER_KHRONOS_validation",
        };
        debugLayers(validationLayers, 1, window, &createInfo);

        //Additional extensions on top of glfw
        glfwExtensions[glfwExtensionCount++] = "VK_EXT_debug_utils";
    #else
        printf("Release Mode\n");
        createInfo.enabledLayerCount = 0; //What gloabls layers are enabled
    #endif

    //More createInfo for extensions
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    //Vulkan instance for the current program, connects library and the API
    VkInstance vkInstance; //This could become tempermental in the future so if release crashes insert: VK_OBJECT_TYPE_INSTANCE and ignore compiler cries
    VkResult instanceResult = vkCreateInstance(&createInfo, NULL, &vkInstance);
    if (instanceResult != VK_SUCCESS) {
        printf("Failed to create instance, code: %d\n", instanceResult);
        return NULL;
    }

    //Adding the debug mode logging layer
    #if DEBUG_MODE == 1
        if (addDebugMessenger(vkInstance, debugMessenger, NULL) != VK_SUCCESS)
        {
            printf("Debug callback creation failed! Code: \n");
            glfwClean(window);
            return 0;
        }
    #endif

    return vkInstance;
}

VkResult deviceInit(VkInstance vkInstance, Device* device, VkPhysicalDevice *physicalDevicePtr) {
    //Handle to a graphsics card to utilize
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    //Grab all the devices on the computer
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, NULL); //TODO: reconfigure so that both of the physical device things are in one call
    if (deviceCount == 0)
    {
        printf("No devices with Vulkan Support!\n");
        return VK_INCOMPLETE;
        //destoryProgram(vkInstance, debugMessenger, window, -1);
    }

    VkPhysicalDevice devices[deviceCount];
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices);
    for (size_t i = 0; i < deviceCount; i++) 
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) //TODO: Search beyond difference between discrete and integrated graphics
        {
            physicalDevice = devices[i];
        }
        else if (physicalDevice == VK_NULL_HANDLE)
        {
            physicalDevice = devices[i];
        }
    } //TODO: Figure out what queue familes are supported by the GPU, currently assuming all needed are present

    uint32_t queueFamilyIndicies;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, NULL);

    VkQueueFamilyProperties queueFamilies[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies);

    for (size_t i = 0; i < queueFamilyCount; i++)
    {
        if (queueFamilies[i].queueFlags == VK_QUEUE_GRAPHICS_BIT)
        {
            queueFamilyIndicies = i;
        }
    }

    VkDeviceQueueCreateInfo queueCreateInfo = {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamilyIndicies;
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    
    VkPhysicalDeviceFeatures deviceFeatures = {};

    const char *deviceExtentions[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkDeviceCreateInfo deviceCreateInfo = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.enabledExtensionCount = 1;
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtentions;
    //TODO: Backward compat with device specific layers

    //INFO: Querying for swapchain info
    

    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, NULL, &(device->vkDevice)) != VK_SUCCESS)
    {
        printf("Failed to create device!\n");
        return VK_INCOMPLETE;
        //destoryProgram(vkInstance, debugMessenger, window, -1);
    }
    vkGetDeviceQueue(device->vkDevice, queueFamilyIndicies, 0,  &(device->vkQueue));

    *physicalDevicePtr = physicalDevice;
    return VK_SUCCESS;
}
