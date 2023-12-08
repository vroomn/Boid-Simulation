#include "vulkanDebug.h"
#include "glfw/glfwFuncs.h"

#include <stdio.h>
#include <string.h>
#include <vulkan/vulkan_core.h>

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    switch (messageSeverity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: //High severity
        fprintf(stderr, "\033[1;31mCRITICAL ERROR: %s\033[1;0m\n", pCallbackData->pMessage);
        break;
    
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: //Low severity:
        fprintf(stderr, "\033[1;33mPOSSIBLE ERROR: %s\033[1;0m\n", pCallbackData->pMessage);
        break;
        
    default:
        break;
    }
    return VK_FALSE;
}

void DestroyDebugUtilsMessengerEXT(VkInstance vkInstance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkInstance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != NULL)
    {
        func(vkInstance, debugMessenger, pAllocator);
    }
}

/* Takes in wanted validation layers and checks that the system supports them, then adds them to the instance
 * @param validationLayers: Array of strings that are valid validation layers
 * @param numLayers: How many layers are in the validationLayers array
 * @param window: The GLFW window that the program is going to bind to (used for shutdown)
 * @param instanceInfo: The instance info as a pointer to modify
 * */
void debugLayers(const char *validationLayers[], int numLayers, GLFWwindow *window, VkInstanceCreateInfo *instanceInfo) {

    //Number of availible debug layers on the system
    uint32_t availibleLayersCount;
    vkEnumerateInstanceLayerProperties(&availibleLayersCount, NULL); //Just queries for how many layers there are
    
    //What debug layers are availible
    VkLayerProperties availibleLayers[availibleLayersCount];
    vkEnumerateInstanceLayerProperties(&availibleLayersCount, availibleLayers);

    //Enumerates through every requested layer passed to the function and checks if it's present
    for (int layerIter = 0; layerIter < numLayers; layerIter++) {
        //Flag to indicate if layer has been found
        int layerFound = 0;
        const char *currentLayer = validationLayers[layerIter];

        for (int possibleLayerIter = 0; possibleLayerIter < availibleLayersCount; possibleLayerIter++) {
            if (strcmp(currentLayer, availibleLayers[possibleLayerIter].layerName)) {
                //Essentially set flag to true
                layerFound = 1;
            }
        }

        //If the layer isn't present end the program
        if (layerFound != 1) {
            printf("Validation layer '%s' not on system!\n", currentLayer);
            glfwClean(window);
            return;
        }
    }

    //Modify the create info for the layer to include validation layers
    instanceInfo->enabledLayerCount = numLayers;
    instanceInfo->ppEnabledLayerNames = validationLayers;

    return;
}

/* Sets up the debug messaging system with configs and creates a new messenger
 * @param vkInstance: The instance to be debuugged
 * @param debugMessenger: The debug messenger pointer to attach to
 * @param pAllocator: Part of the Vulkan allocation system (if unsure set to NULL)
 * */
VkResult addDebugMessenger(VkInstance vkInstance, VkDebugUtilsMessengerEXT *debugMessenger, VkAllocationCallbacks *pAllocator) {
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};

    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = NULL;
    
    PFN_vkCreateDebugUtilsMessengerEXT func = ((PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkInstance, "vkCreateDebugUtilsMessengerEXT"));

    if (func != NULL) {
        return func(vkInstance, &createInfo, pAllocator, debugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}
