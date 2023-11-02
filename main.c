#include <stdio.h>
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

int main(void) {
    if (!glfwInit())
    {
        printf("failed");
    }


    if (glfwVulkanSupported()) {
        printf("worked");
    }

    glfwTerminate();

    return 0;
}