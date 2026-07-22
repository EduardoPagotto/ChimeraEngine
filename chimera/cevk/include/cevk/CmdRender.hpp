#pragma once

#include "Sync.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {

    class CmdRender {
      public:
        explicit CmdRender() = default;
        virtual ~CmdRender() = default;

        CmdRender(const CmdRender&) = delete;
        CmdRender& operator=(const CmdRender&) = delete;

        void begin(VkCommandBuffer cmdBuffer, VkCommandBufferUsageFlagBits flag,
                   const VkRenderPassBeginInfo& renderPassBeginInfo, VkPipeline& graphicPipeline);

        void pushConstants(VkPipelineLayout pipelineLayout, VkShaderStageFlagBits stage, uint32_t offset, size_t size,
                           const void* src);
        void addVertexBuffer(const VkDeviceSize& offset, const VkBuffer& buffer);
        void bindIndexBuffer(const VkBuffer& indexBuffer, const VkDeviceSize& offset);
        void bindVertexBuffer(uint32_t starts);
        void addDescriptorSet(const VkDescriptorSet& desc);
        void bindDescriptorSets(const VkPipelineLayout& pipelineLayout);
        void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset,
                         uint32_t firstInstance);
        void end();
        void clearTemps();

        void submitToRender(VkQueue queue, Sync& sync, const VkPipelineStageFlagBits& pipelineStageFlags);

      private:
        std::vector<VkBuffer> vextexBuffers;
        std::vector<VkDeviceSize> offsets;
        std::vector<VkDescriptorSet> descriptorSetGroup;

        VkCommandBuffer cmdBuffer{VK_NULL_HANDLE};
    };
} // namespace ce
