#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "vulkanInit.h"
#include "vulkanDebug.h"
#include "glfwFuncs.h"
#include <stdio.h>
#include <string.h>

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
            "VK_LAYER_KHRONOS_validation"
        };

        uint32_t validationLayerCount = sizeof(validationLayers)/sizeof(validationLayers[0]);

        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, NULL);
        
        VkLayerProperties availableLayers[layerCount];
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

        for (size_t i = 0; i < validationLayerCount; i++) {
            int layerFound = 0;
            const char *currentLayer = validationLayers[i];

            for (size_t j = 0; j < sizeof(availableLayers)/sizeof(availableLayers[0]); j++) {
                if (strcmp(currentLayer, (const char *)availableLayers[j].layerName) ) //Dunno if the extra cast is needed but dont trust it anyway, I hate strings
                {
                    layerFound = 1;
                }
            }

            if (layerFound != 1)
            {
                printf("Validation layers not availible!\n");
                glfwClean(window);
                return 0;
            }

            printf("Layers found!\n");

        }
        createInfo.enabledLayerCount = validationLayerCount;
        createInfo.ppEnabledLayerNames = validationLayers;

        //Debug Messages to the stdout
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
        debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugCreateInfo.pfnUserCallback = debugCallback;
        debugCreateInfo.pUserData = NULL;

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
        if (CreateDebugUtilsMessengerEXT(vkInstance, &debugCreateInfo, NULL, debugMessenger) != VK_SUCCESS)
        {
            printf("Debug callback creation failed! Code: \n");
            glfwClean(window);
            return 0;
        }
    #endif

    return vkInstance;
}