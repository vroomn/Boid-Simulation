#include <stdio.h>

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

int main(void) {
    //Initalization of libraries
    //Begin the GLFW library
    if (!glfwInit())
    {
        printf("failed");
        return -1;
    }

    //Simple check to see if Vulkan is accessible on the device and build system
    if (!glfwVulkanSupported()) {
        printf("Requires Vulkan to display grpahics, please check drivers and SDK if in development\n");
        return -1;
    }

    //Main code


    //Cleanup GLFW
    glfwTerminate();

    return 0;
}