#pragma once

#include "Shader.hpp"
#include <memory>

namespace ce {

#pragma region Pipeline

    class Pipeline {
      public:
        explicit Pipeline(VkDevice device) : device_(device) {}
        virtual ~Pipeline();

        void add_viewport(const VkViewport& viewport) { viewports_.push_back(viewport); }
        void add_scissor(const VkRect2D scissor) { scissors_.push_back(scissor); }
        void add_dynamic_state_enables(const VkDynamicState& state) { dynamic_state_enables_.push_back(state); };
        void add_colour_state(const VkPipelineColorBlendAttachmentState& colour_state) {
            colour_states_.push_back(colour_state);
        }

        void create(std::shared_ptr<Shader> shader, VkRenderPass render_pass, VkPipelineLayout pipeline_layout);

        VkPipeline& get() { return handle_; }

      private:
        VkDevice device_{VK_NULL_HANDLE};
        VkPipeline handle_{VK_NULL_HANDLE};

        std::vector<VkViewport> viewports_;
        std::vector<VkRect2D> scissors_;
        std::vector<VkDynamicState> dynamic_state_enables_;
        std::vector<VkPipelineColorBlendAttachmentState> colour_states_;
    };

#pragma endregion

#pragma region PipelineLayout

    class PipelineLayout {

      public:
        PipelineLayout(VkDevice device) : device_(device) {}

        virtual ~PipelineLayout() { vkDestroyPipelineLayout(device_, handle_, nullptr); }

        void add_layout(const VkDescriptorSetLayout& descriptor_set_layout) {
            descriptor_set_layouts_.push_back(descriptor_set_layout);
        }
        void add_push_range(const VkPushConstantRange& push_constant_range) {
            push_constant_ranges_.push_back(push_constant_range);
        }

        VkPipelineLayout& get() { return handle_; }

        void create();

      private:
        VkDevice device_{VK_NULL_HANDLE};
        VkPipelineLayout handle_{VK_NULL_HANDLE};
        std::vector<VkDescriptorSetLayout> descriptor_set_layouts_;
        std::vector<VkPushConstantRange> push_constant_ranges_;
    };
#pragma endregion
} // namespace ce
