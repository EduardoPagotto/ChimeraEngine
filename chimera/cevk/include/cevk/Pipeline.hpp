#ifndef PIPELINE_CLASS_HPP
#define PIPELINE_CLASS_HPP

#include "ShaderModule.hpp"
#include <memory>

namespace ce {

#pragma region Pipeline

    class Pipeline {
      public:
        explicit Pipeline(VkDevice device) : device(device) {}
        virtual ~Pipeline();

        void addViewport(const VkViewport& viewport) { this->viewports.push_back(viewport); }
        void addScissor(const VkRect2D scissor) { this->scissors.push_back(scissor); }
        void addDynamicStateEnables(const VkDynamicState& state) { dynamicStateEnables.push_back(state); };
        void addColourState(const VkPipelineColorBlendAttachmentState& colourState) {
            this->colourStates.push_back(colourState);
        }

        void create(std::shared_ptr<ShaderModule> shaderModule, VkRenderPass renderPass,
                    VkPipelineLayout pipelineLayout);

        VkPipeline& get() { return this->handle; }

      private:
        VkDevice device{VK_NULL_HANDLE};
        VkPipeline handle{VK_NULL_HANDLE};

        std::vector<VkViewport> viewports;
        std::vector<VkRect2D> scissors;
        std::vector<VkDynamicState> dynamicStateEnables;
        std::vector<VkPipelineColorBlendAttachmentState> colourStates;
    };

#pragma endregion

#pragma region PipelineLayout

    class PipelineLayout {

      public:
        PipelineLayout(VkDevice device) : device(device) {}

        virtual ~PipelineLayout() { vkDestroyPipelineLayout(device, this->handle, nullptr); }

        void addLayout(const VkDescriptorSetLayout& descriptorSetLayout) {
            this->descriptorSetLayouts.push_back(descriptorSetLayout);
        }
        void addPushRange(const VkPushConstantRange& pushConstantRange) {
            this->pushConstantRanges.push_back(pushConstantRange);
        }

        VkPipelineLayout& get() { return this->handle; }

        void create();

      private:
        VkDevice device{VK_NULL_HANDLE};
        VkPipelineLayout handle{VK_NULL_HANDLE};
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
        std::vector<VkPushConstantRange> pushConstantRanges;
    };
#pragma endregion
} // namespace ce
#endif
