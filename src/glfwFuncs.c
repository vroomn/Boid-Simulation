#include "GLFW/glfw3.h"

#include "glfwFuncs.h"
#include <stdio.h>

/*Starts GLFW, checks if Vulkan is supported, and creates a new window object
* @param width: Width of the window
* @param height: Height of the window
* @param windowName: Name of the window (will be on the title bar)
* @return A pointer to a GLFW window object
*/
GLFWwindow* glfwWindowInit(int width, int height, const char* windowName) {
    //Initalize GLFW
    if (!glfwInit()) {
        printf("GLFW init failed!\n");
        return NULL;
    }
    
    //Verify that Vulkan is supported on the device
    if (!glfwVulkanSupported()) {
        printf("Vulkan is not supported, check drivers or SDK\n");
        return NULL;
    }
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    return glfwCreateWindow(width, height, windowName, NULL, NULL);
}

/*Cleans up the memory and other resources used by GLFW (Use this only when the program is shutting down)
* @param window: The GLFW window object to be destroyed
*/
void glfwClean(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();
    return;
}