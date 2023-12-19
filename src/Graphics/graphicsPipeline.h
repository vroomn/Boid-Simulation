#ifndef GRAPHICSPIPELINE_H
#define GRAPHICSPIPELINE_H

#include <stdint.h>
#include <vulkan/vulkan_core.h>

//Stores the data and how much there is
typedef struct {
    char *data;
    long size;
} shaderFileData;

void createGraphicsPipeline(VkDevice device, VkExtent2D extent, VkPipelineLayout *pipelineLayout);

#endif
