#pragma once

#include "Frame.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {

    class CmdRender {
      public:
        explicit CmdRender() = default;
        virtual ~CmdRender() = default;

        CmdRender(const CmdRender&) = delete;
        CmdRender& operator=(const CmdRender&) = delete;

        void begin(VkCommandBuffer cmdbuffer, VkCommandBufferUsageFlagBits flag,
                   const VkRenderPassBeginInfo& renderpass_begin_info, VkPipeline& graphic_pipeline);

        void push_constants(VkPipelineLayout pipeline_layout, VkShaderStageFlagBits stage, uint32_t offset, size_t size,
                            const void* src);
        void add_vertex_buffer(const VkDeviceSize& offset, const VkBuffer& buffer);
        void bind_index_buffer(const VkDeviceSize& offset, const VkBuffer& index_buffer);
        void bind_vertex_buffer(uint32_t starts);
        void add_descriptor_set(const VkDescriptorSet& desc);
        void bind_descriptor_sets(const VkPipelineLayout& pipeline_layout);
        void draw_indexed(uint32_t index_count, uint32_t instance_count, uint32_t first_index, int32_t vertex_offset,
                          uint32_t first_instance);
        void end();
        void clear_temps();

        void submit_to_render(VkQueue queue, Frame* frame, const VkPipelineStageFlagBits& pipeline_stage_flags);

      private:
        std::vector<VkBuffer> vextex_buffers_;
        std::vector<VkDeviceSize> offsets_;
        std::vector<VkDescriptorSet> descriptorset_group_;

        VkCommandBuffer cmdbuffer_{VK_NULL_HANDLE};
    };
} // namespace ce
