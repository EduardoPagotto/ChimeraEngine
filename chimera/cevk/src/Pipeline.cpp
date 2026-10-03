#include "Pipeline.hpp"
namespace ce {

#pragma region Pipeline

    Pipeline::~Pipeline() { vkDestroyPipeline(device_, this->handle_, nullptr); }

    void Pipeline::create(std::shared_ptr<Shader> shader, VkRenderPass render_pass, VkPipelineLayout pipeline_layout) {

        // -- VIEWPORT & SCISSOR
        const VkPipelineViewportStateCreateInfo viewport_state_create_info{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = static_cast<uint32_t>(this->viewports_.size()),
            .pViewports = this->viewports_.data(),
            .scissorCount = static_cast<uint32_t>(this->scissors_.size()),
            .pScissors = this->scissors_.data()};

        // Dynamic State creation info
        const VkPipelineDynamicStateCreateInfo dynamic_state_create_info{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .dynamicStateCount = static_cast<uint32_t>(dynamic_state_enables_.size()),
            .pDynamicStates = dynamic_state_enables_.data()};

        // -- RASTERIZER --
        const VkPipelineRasterizationStateCreateInfo rasterization_create_info{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .depthClampEnable =
                VK_FALSE, // Change if fragments beond near/far planes are clipped (default) of clamped to plane
            .rasterizerDiscardEnable =
                VK_FALSE, // Whether to diacard data and skip rasterization. Never
                          // create fragments, only suitable for pipeline without frambuffer output
            .polygonMode = VK_POLYGON_MODE_FILL,          // How to handle filling points beteen vertices
            .cullMode = VK_CULL_MODE_BACK_BIT,            // Whitch face of a tri to cull(nao desenha a backface)
            .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE, // kinding to determine side is front
            .depthBiasClamp =
                VK_FALSE, // Whether to add depth bias to fragment (good for stopping "swadow acne" in shadow mapping)
            .lineWidth = 1.0F // How thick lines shoud be when draw
        };

        // -- MULTISMAPLING --
        const VkPipelineMultisampleStateCreateInfo multisampling_create_info{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT, // Number of sample to use per fragment
            .sampleShadingEnable = VK_FALSE                // Enable multisample shading or not
        };

        // -- BLENDING --
        // Blending decide how to blend a new colour being written to a fragment, whit the old value
        const VkPipelineColorBlendStateCreateInfo color_blending_create_info{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO, //
            .logicOpEnable = VK_FALSE, // alternative to calulation is use logical operations
            .attachmentCount = static_cast<uint32_t>(this->colour_states_.size()),
            .pAttachments = this->colour_states_.data()};

        // -- DEPTH STENCIL TESTING
        const VkPipelineDepthStencilStateCreateInfo depth_stencil_create_info{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .depthTestEnable = VK_TRUE,           // Enable checking depth to determine fragment write
            .depthWriteEnable = VK_TRUE,          // Enable writing to depth buffer (to replace all values)
            .depthCompareOp = VK_COMPARE_OP_LESS, // Coparison operation that allows an overwrite (is in front)
            .depthBoundsTestEnable = VK_FALSE,    // Depth Bonds test: Does the depth value exist between two bounds
            .stencilTestEnable = VK_FALSE         // Enable stencil test
        };

        // -- GRAPHICS PIPELINE CREATION
        const VkGraphicsPipelineCreateInfo pipeline_create_info{
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount = static_cast<uint32_t>(shader->get_shader_create_infos().size()), // numberr of shader stages
            .pStages = shader->get_shader_create_infos().data(),                           // List of shader stages
            .pVertexInputState = shader->getp_vertex_input_create_info(), // All the fixed function pipeline states
            .pInputAssemblyState = shader->getp_input_assembly(),         //
            .pViewportState = &viewport_state_create_info,                //
            .pRasterizationState = &rasterization_create_info,            //
            .pMultisampleState = &multisampling_create_info,              //
            .pDepthStencilState = &depth_stencil_create_info,             //
            .pColorBlendState = &color_blending_create_info,              //
            .pDynamicState = &dynamic_state_create_info,                  //
            .layout = pipeline_layout,                                    // Pipeline Layout shoud use
            .renderPass = render_pass,            // Render pass description the pipelineis compatible with
            .subpass = 0,                         // Subpass of render pass to use with pipeline
            .basePipelineHandle = VK_NULL_HANDLE, // Existing pipeline to derive from...
            .basePipelineIndex =
                -1 // or index of pipeline being created to derive from (in case creating multiple at once)
        };

        // Create Graphics Pipeline
        if (vkCreateGraphicsPipelines(device_, VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &this->handle_) !=
            VK_SUCCESS) {
            throw std::runtime_error("Failed to create a graphic pipeline");
        }
    }

#pragma endregion

#pragma region PipelineLayout
    // -- PipelineLayout

    void PipelineLayout::create() {
        const VkPipelineLayoutCreateInfo pipeline_layout_create_info{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = static_cast<uint32_t>(descriptor_set_layouts_.size()),
            .pSetLayouts = descriptor_set_layouts_.data(),
            .pushConstantRangeCount = static_cast<uint32_t>(this->push_constant_ranges_.size()),
            .pPushConstantRanges = this->push_constant_ranges_.data()};

        // Create PipelineLayout
        if (vkCreatePipelineLayout(device_, &pipeline_layout_create_info, nullptr, &this->handle_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Pipeline Layout!");
        }
    }
#pragma endregion
} // namespace ce
