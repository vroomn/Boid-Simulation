/* EXAMPLES AND MAJORITY OF BOILERPLATE TAKEN OR MIDIFIED FROM THE VULKAN API DOCUMENTATION TUTORIAL */

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <stdlib.h>
#include "glfwFuncs.h"
#include "vulkan_src/vulkanDebug.h"
#include "vulkan_src/vulkanInit.h"

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

int main(void) {
    //GLFW window object to refrence by the libary
    GLFWwindow* window = glfwWindowInit(640, 480, "Boid Simulation");
    if (window == NULL) {return -1;}

    //Create the vulkan needed systems
    /*Messenger to use on dubug log*/
    VkDebugUtilsMessengerEXT debugMessenger;
    VkInstance vkInstance = vulkanInit(window, &debugMessenger);
    if (vkInstance == NULL) {return -1;}

    Device graphicsDevice = deviceInit(vkInstance);

    //Main loop
    while (!glfwWindowShouldClose(window)) {
        //Get events and process them for glfw
        glfwPollEvents();
    }
    
    vkDestroyDevice(graphicsDevice.vkDevice, NULL);
    destoryProgram(vkInstance, debugMessenger, window, 0);

    return 0;
}