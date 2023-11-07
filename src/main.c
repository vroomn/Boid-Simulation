/* EXAMPLES AND MAJORITY OF BOILERPLATE TAKEN OR MIDIFIED FROM THE VULKAN API DOCUMENTATION TUTORIAL */

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <stdio.h>
#include <stdlib.h>
#include "glfwFuncs.h"
#include "vulkan_src/vulkanDebug.h"
#include "vulkan_src/vulkanInit.h"

void destoryProgram(VkInstance vkInstance, VkDebugUtilsMessengerEXT debugMessenger, GLFWwindow* window, int exitNum);

int main(void) {
    //GLFW window object to refrence by the libary
    GLFWwindow* window = glfwWindowInit(640, 480, "Boid Simulation");
    if (window == NULL) {return -1;}

    //Create the vulkan needed systems
    /*Messenger to use on dubug log*/
    VkDebugUtilsMessengerEXT debugMessenger;
    VkInstance vkInstance = vulkanInit(window, &debugMessenger);
    if (vkInstance == NULL) {return -1;}

    //Holds the device and queue for the graphics pipeline
    Device graphicsDevice;
    if (deviceInit(vkInstance, &graphicsDevice) == VK_INCOMPLETE) {
        destoryProgram(vkInstance, debugMessenger, window, -1);
    };

    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.hwnd = glfwGetWin32Window(window);
    surfaceCreateInfo.hinstance = GetModuleHandle(NULL);

    VkSurfaceKHR surface;
    if (vkCreateWin32SurfaceKHR(vkInstance, &surfaceCreateInfo, NULL, &surface) != VK_SUCCESS)
    {
        printf("Failed to create surface!\n");
        destoryProgram(vkInstance, debugMessenger, window, -1);
    }

    //Main loop
    while (!glfwWindowShouldClose(window)) {
        //Get events and process them for glfw
        glfwPollEvents();
    }
    
    vkDestroyDevice(graphicsDevice.vkDevice, NULL);
    vkDestroySurfaceKHR(vkInstance ,surface, NULL);
    destoryProgram(vkInstance, debugMessenger, window, 0);

    return 0;
}

void destoryProgram(VkInstance vkInstance, VkDebugUtilsMessengerEXT debugMessenger, GLFWwindow* window, int exitNum) {
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