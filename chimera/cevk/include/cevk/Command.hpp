#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {

    class Command {
      public:
        explicit Command(VkCommandBuffer cmdBuffer, VkCommandBufferUsageFlagBits flag);
        virtual ~Command();

        Command(const Command&) = delete;
        Command& operator=(const Command&) = delete;

        void init(VkCommandBuffer cmdBuffer, VkCommandBufferUsageFlagBits flag);
        void destroy();
        void beginAndPipeline(const VkRenderPassBeginInfo& renderPassBeginInfo, VkPipeline& graphicPipeline);
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

      private:
        std::vector<VkBuffer> vextexBuffers;
        std::vector<VkDeviceSize> offsets;
        std::vector<VkDescriptorSet> descriptorSetGroup;

        VkCommandBuffer cmdBuffer{VK_NULL_HANDLE};
    };
} // namespace ce
