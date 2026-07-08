#include "Pipeline.hpp"
namespace ce {

    Pipeline::~Pipeline() {
        vkDestroyPipeline(device, this->graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(device, this->pipelineLayout, nullptr);
    }

    void Pipeline::create(std::shared_ptr<ShaderModule> shaderModule, VkRenderPass renderPass) { // NOLINT
        // PipelineLayout
        const VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                                                                  .setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size()),
                                                                  .pSetLayouts = descriptorSetLayouts.data(),
                                                                  .pushConstantRangeCount =
                                                                      static_cast<uint32_t>(this->pushConstantRanges.size()),
                                                                  .pPushConstantRanges = this->pushConstantRanges.data()};

        // Create PipelineLayout
        if (vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &this->pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Pipeline Layout!");
        }

        // -- VIEWPORT & SCISSOR
        const VkPipelineViewportStateCreateInfo viewportStateCreateInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                                                                        .viewportCount = static_cast<uint32_t>(this->viewports.size()),
                                                                        .pViewports = this->viewports.data(),
                                                                        .scissorCount = static_cast<uint32_t>(this->scissors.size()),
                                                                        .pScissors = this->scissors.data()};

        // Dynamic State creation info
        const VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                                                                      .dynamicStateCount =
                                                                          static_cast<uint32_t>(dynamicStateEnables.size()),
                                                                      .pDynamicStates = dynamicStateEnables.data()};

        // -- RASTERIZER --
        const VkPipelineRasterizationStateCreateInfo rasterizationCreateInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .depthClampEnable = VK_FALSE,        // Change if fragments beond near/far planes are clipped (default) of clamped to plane
            .rasterizerDiscardEnable = VK_FALSE, // Whether to diacard data and skip rasterization. Never
                                                 // create fragments, only suitable for pipeline without frambuffer output
            .polygonMode = VK_POLYGON_MODE_FILL, // How to handle filling points beteen vertices
            .cullMode = VK_CULL_MODE_BACK_BIT,   // Whitch face of a tri to cull(nao desenha a backface)
            .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE, // kinding to determine side is front
            .depthBiasClamp = VK_FALSE, // Whether to add depth bias to fragment (good for stopping "swadow acne" in shadow mapping)
            .lineWidth = 1.0F           // How thick lines shoud be when draw
        };

        // -- MULTISMAPLING --
        const VkPipelineMultisampleStateCreateInfo multisamplingCreateInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT, // Number of sample to use per fragment
            .sampleShadingEnable = VK_FALSE                // Enable multisample shading or not
        };

        // -- BLENDING --
        // Blending decide how to blend a new colour being written to a fragment, whit the old value
        const VkPipelineColorBlendStateCreateInfo colorBlendingCreateInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO, //
            .logicOpEnable = VK_FALSE,                                         // alternative to calulation is use logical operations
            .attachmentCount = static_cast<uint32_t>(this->colourStates.size()),
            .pAttachments = this->colourStates.data()};

        // -- DEPTH STENCIL TESTING
        const VkPipelineDepthStencilStateCreateInfo depthStencilCreateInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .depthTestEnable = VK_TRUE,           // Enable checking depth to determine fragment write
            .depthWriteEnable = VK_TRUE,          // Enable writing to depth buffer (to replace all values)
            .depthCompareOp = VK_COMPARE_OP_LESS, // Coparison operation that allows an overwrite (is in front)
            .depthBoundsTestEnable = VK_FALSE,    // Depth Bonds test: Does the depth value exist between two bounds
            .stencilTestEnable = VK_FALSE         // Enable stencil test
        };

        // -- GRAPHICS PIPELINE CREATION
        const VkGraphicsPipelineCreateInfo pipelineCreateInfo{
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount = static_cast<uint32_t>(shaderModule->getShaderCreateInfos().size()), // numberr of shader stages
            .pStages = shaderModule->getShaderCreateInfos().data(),                           // List of shader stages
            .pVertexInputState = shaderModule->getpVertexInputCreateInfo(),                   // All the fixed function pipeline states
            .pInputAssemblyState = shaderModule->getpInputAssembly(),                         //
            .pViewportState = &viewportStateCreateInfo,                                       //
            .pRasterizationState = &rasterizationCreateInfo,                                  //
            .pMultisampleState = &multisamplingCreateInfo,                                    //
            .pDepthStencilState = &depthStencilCreateInfo,                                    //
            .pColorBlendState = &colorBlendingCreateInfo,                                     //
            .pDynamicState = &dynamicStateCreateInfo,                                         //
            .layout = pipelineLayout,                                                         // Pipeline Layout shoud use
            .renderPass = renderPass,             // Render pass description the pipelineis compatible with
            .subpass = 0,                         // Subpass of render pass to use with pipeline
            .basePipelineHandle = VK_NULL_HANDLE, // Existing pipeline to derive from...
            .basePipelineIndex = -1               // or index of pipeline being created to derive from (in case creating multiple at once)
        };

        // Create Graphics Pipeline
        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &this->graphicsPipeline) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a graphic pipeline");
        }
    }

} // namespace ce
