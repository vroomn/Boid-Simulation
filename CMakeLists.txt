cmake_minimum_required(VERSION 3.17)

project(
    BoidsVulkan
    LANGUAGES C)

add_compile_definitions(DEBUG_MODE=1) #Defaults to release mode when removed or set to 0 (0=Release Mode :: 1=Debug Mode) (opt in)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(Vulkan REQUIRED)

add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}\\libs\\glfw-3.3.8
    )

set(SOURCE_FILES 
    src/main.c
    src/glfw/glfwFuncs.c
    src/Initalization/vulkanInit.c
    src/Initalization/swapchain.c
    src/DebugTools/vulkanDebug.c)

add_executable(main ${SOURCE_FILES})

target_include_directories(
    main
    PRIVATE
    src/
    libs/glfw-3.3.8/include/
    /VulkanSDK/1.3.268.0/Include
)

target_link_libraries(
    main
    PUBLIC
    glfw
    Vulkan::Vulkan
)
