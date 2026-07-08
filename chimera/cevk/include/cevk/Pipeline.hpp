#pragma once

#include "entitys/ShaderModule.hpp"
#include <memory>

namespace ce {

    class Pipeline {
      public:
        explicit Pipeline(VkDevice device) : device(device) {}
        virtual ~Pipeline();

        void addLayout(const VkDescriptorSetLayout& descriptorSetLayout) { this->descriptorSetLayouts.push_back(descriptorSetLayout); }
        void addPushRange(const VkPushConstantRange& pushConstantRange) { this->pushConstantRanges.push_back(pushConstantRange); }
        void addViewport(const VkViewport& viewport) { this->viewports.push_back(viewport); }
        void addScissor(const VkRect2D scissor) { this->scissors.push_back(scissor); }
        void addDynamicStateEnables(const VkDynamicState& state) { dynamicStateEnables.push_back(state); };
        void addColourState(const VkPipelineColorBlendAttachmentState& colourState) { this->colourStates.push_back(colourState); }

        void create(std::shared_ptr<ShaderModule> shaderModule, VkRenderPass renderPass);

        VkPipelineLayout& getPipelineLayout() { return this->pipelineLayout; }
        VkPipeline& getGraphicsPipeline() { return this->graphicsPipeline; }

      private:
        VkDevice device;
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
        std::vector<VkPushConstantRange> pushConstantRanges;
        std::vector<VkViewport> viewports;
        std::vector<VkRect2D> scissors;
        std::vector<VkDynamicState> dynamicStateEnables;
        std::vector<VkPipelineColorBlendAttachmentState> colourStates;
    };
} // namespace ce
