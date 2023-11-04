#include <stdio.h>

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

int main(void) {
    //Initalization of libraries
    //Begin the GLFW library
    if (!glfwInit()) {
        printf("failed");
        return -1;
    }

    //Simple check to see if Vulkan is accessible on the device and build system
    if (!glfwVulkanSupported()) {
        printf("Requires Vulkan to display grpahics, please check drivers and SDK if in development\n");
        return -1;
    }

    //Creation of the window
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //Disables resizing the window
    GLFWwindow* window = glfwCreateWindow(640, 480, "Boid Simulation", NULL, NULL);

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
    //Number of global validation layers
    uint32_t glfwExtensionCount = 0;
    createInfo.ppEnabledExtensionNames = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.enabledLayerCount = 0; //What gloabls layers are enabled

    //Vulkan instance for the current program, connects library and the API
    VkInstance vkInstance; //This could become tempermental in the future so if release crashes insert: VK_OBJECT_TYPE_INSTANCE and ignore compiler cries
    VkResult instanceResult = vkCreateInstance(&createInfo, NULL, &vkInstance);
    if (instanceResult != VK_SUCCESS) {
        printf("Failed to create instance, code: %d\n", instanceResult);
    }

    //Main loop
    while (!glfwWindowShouldClose(window)) {
        //Get events and process them for glfw
        glfwPollEvents();
    }
    
    //Cleanup Vulkan
    vkDestroyInstance(vkInstance, NULL);

    //Cleanup GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}