cmake_minimum_required(VERSION 3.17)

project(
    BoidsVulkan
    LANGUAGES C)


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

#option(TRACY_ENABLE "" ON)
#option(TRACY_ON_DEMAND "" ON)



add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}\\libs\\glfw-3.3.8
    )

add_executable(main src/main.c)

target_include_directories(
    main
    PUBLIC
    libs/glfw-3.3.8/include
    /VulkanSDK/1.3.268.0/Include
)

target_link_libraries(
    main
    PUBLIC
    glfw
)