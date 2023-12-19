#include "Graphics/graphicsPipeline.h"
#include <minwindef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan_core.h>

/* Gets the data from a binary file in the Shaders directory (pulls a shader)
 * @param filename: name of the file to search for
 * @param dataBuffer: pointer to buffer of data within the shaderfile
 * */
void getShaderFile(const char *filename, shaderFileData *dataBuffer);

VkShaderModule createShaderModule(shaderFileData *shaderData, VkDevice device);

void createGraphicsPipeline(VkDevice device, VkExtent2D extent, VkPipelineLayout *pipelineLayout) {
    shaderFileData vertShaderBuffer;
    getShaderFile("vert.spv", &vertShaderBuffer);
    shaderFileData fragShaderBuffer;
    getShaderFile("frag.spv", &fragShaderBuffer);

    VkShaderModule vertShaderModule = createShaderModule(&vertShaderBuffer, device);
    VkShaderModule fragShaderModule = createShaderModule(&fragShaderBuffer, device);
    free(vertShaderBuffer.data);
    free(fragShaderBuffer.data);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo;
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";
    vertShaderStageInfo.pSpecializationInfo = NULL;
    vertShaderStageInfo.pNext = NULL;

    VkPipelineShaderStageCreateInfo fragShaderStageInfo;
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";
    fragShaderStageInfo.pSpecializationInfo = NULL;
    fragShaderStageInfo.pNext = NULL;

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    //Format of the vertex data for vertex shaders
    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = NULL;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = NULL;
    vertexInputInfo.pNext = NULL;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly;
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    inputAssembly.pNext = NULL;

//INFO: Both the viewport and scissor could be updated in the main loop just not going to cuz I don't wanna

    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)extent.width;
    viewport.height = (float)extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor;
    VkOffset2D offset = {0, 0};
    scissor.offset = offset;
    scissor.extent = extent;

    VkPipelineViewportStateCreateInfo viewportState;
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;
    viewportState.pNext = NULL;

    VkPipelineRasterizationStateCreateInfo rasterizer;
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo multisampling;
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = NULL;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending;
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;
    colorBlending.pNext = NULL;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = NULL;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = NULL;
    pipelineLayoutInfo.pNext = NULL;

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, NULL, pipelineLayout) != VK_SUCCESS) {
        printf("FAILED TO CREATE PIPELINE LAYOUT");
        return;
    }

    vkDestroyShaderModule(device, vertShaderModule, NULL);
    vkDestroyShaderModule(device, fragShaderModule, NULL);
    return;
}

//TODO: Create a return system for failures and successes

//Local function so it only gets defined at the top
void getShaderFile(const char *filename, shaderFileData *dataBuffer) {
    //Relative path to the shaders
    char relPath[MAX_PATH] = "src/Graphics/Shaders/";
    strncat(relPath, filename, strlen(filename)); //Appends the passed file to the relative path

    //Holds the absolute path from disk
    char fullPath[MAX_PATH] = {};
    //Didn't know about this function (https://stackoverflow.com/questions/45124869/cross-platform-alternative-to-this-realpath-definition)
    _fullpath(fullPath, relPath, MAX_PATH); //Finds the source directory of the program
    
    FILE *file;
    file = fopen(fullPath, "rb");
    if (file == NULL) {
        printf("Failed to open file: %s", filename);
        fclose(file);
        return;
    }

    //Get the size of the file in bytes (Naman Tamrakar: https://dev.to/namantam1/ways-to-get-the-file-size-in-c-2mag)
    if (fseek(file, 0, SEEK_END) != 0) {
        printf("Failed to seek end of shader file. Name: %s\n", filename);
        fclose(file);
        return;
    }

    dataBuffer->size = ftell(file);
    if (dataBuffer->size == -1L) {
        printf("Failed to get EOF position in file: %s\n", filename);
        fclose(file);
        return;
    }
    fseek(file, 0, SEEK_SET); //Returns to the beginning of the file

    //Fill the buffer with info within the file
    dataBuffer->data = (char *)malloc(dataBuffer->size * sizeof(char));
    fread(dataBuffer->data, dataBuffer->size, 1, file);

    fclose(file);
    return;
}

VkShaderModule createShaderModule(shaderFileData *shaderData, VkDevice device) {
    VkShaderModuleCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderData->size;
    createInfo.pCode = (const uint32_t*)shaderData->data;
    createInfo.pNext = NULL;

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, NULL, &shaderModule)) {
        printf("Unable to create shader module");
        return NULL;
    }

    return shaderModule;
}
