/* EXAMPLES AND MAJORITY OF BOILERPLATE TAKEN OR MIDIFIED FROM THE VULKAN API DOCUMENTATION TUTORIAL */

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <stdlib.h>
#include "glfwFuncs.h"
#include "vulkan_src/vulkanDebug.h"
#include "vulkan_src/vulkanInit.h"

void destoryProgram(VkInstance vkInstance, VkDebugUtilsMessengerEXT debugMessenger, GLFWwindow* window, int exitNum)
{
    //Cleanup Vulkan
    #if DEBUG_MODE == 1
        DestroyDebugUtilsMessengerEXT(vkInstance, debugMessenger, NULL);
    #endif

    vkDestroyInstance(vkInstance, NULL);

    //Cleanup GLFW
    glfwClean(window);
    #if DEBUG_MODE == 1
        getchar(); //Halts program close until there is a user input
    #endif

    exit(exitNum);
    return;
}

int main(void) {
    //GLFW window object to refrence by the libary
    GLFWwindow* window = glfwWindowInit(640, 480, "Boid Simulation");
    if (window == NULL) {return -1;}

    //Create the vulkan needed systems
    /*Messenger to use on dubug log*/
    VkDebugUtilsMessengerEXT debugMessenger;
    VkInstance vkInstance = vulkanInit(window, &debugMessenger);
    if (vkInstance == NULL) {return -1;}

    //Handle to a graphsics card to utilize
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    //Grab all the devices on the computer
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, NULL); //TODO: reconfigure so that both of the physical device things are in one call
    if (deviceCount == 0)
    {
        printf("No devices with Vulkan Support!\n");
        destoryProgram(vkInstance, debugMessenger, window, -1);
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

    VkDeviceCreateInfo deviceCreateInfo = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    //TODO: Backward copat with device specific layers

    VkDevice vkDevice;
    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, NULL, &vkDevice) != VK_SUCCESS)
    {
        printf("Failed to create device!\n");
        destoryProgram(vkInstance, debugMessenger, window, -1);
    }
    VkQueue graphicsQueue;
    vkGetDeviceQueue(vkDevice, queueFamilyIndicies, 0,  &graphicsQueue);

    //Main loop
    while (!glfwWindowShouldClose(window)) {
        //Get events and process them for glfw
        glfwPollEvents();
    }
    
    vkDestroyDevice(vkDevice, NULL);
    destoryProgram(vkInstance, debugMessenger, window, 0);

    return 0;
}