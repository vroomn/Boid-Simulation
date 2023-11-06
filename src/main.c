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

    getchar(); //Halts program close until there is a user input

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

    //Main loop
    while (!glfwWindowShouldClose(window)) {
        //Get events and process them for glfw
        glfwPollEvents();
    }
    
    destoryProgram(vkInstance, debugMessenger, window, 0);

    return 0;
}