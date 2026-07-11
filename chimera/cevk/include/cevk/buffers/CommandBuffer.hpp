#ifndef __COMMANDBUFFER_CLASS_HPP
#define __COMMANDBUFFER_CLASS_HPP

#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {

    struct SubmitToRenderInfo {
        VkQueue gQueue;
        VkSemaphore wait;
        VkSemaphore signal;
        VkFence fence;
        VkPipelineStageFlagBits pipelineStageFlags;
        size_t bufferIndex;
    };

    class CommandBuffer {
      public:
        explicit CommandBuffer(VkDevice device, VkCommandPool commandPool, size_t count);
        virtual ~CommandBuffer();

        CommandBuffer(const CommandBuffer&) = delete;
        CommandBuffer& operator=(const CommandBuffer&) = delete;

        void clean(size_t index);
        void cleanAll();
        void begin(size_t index, VkCommandBufferUsageFlagBits flag);
        void end(size_t index);

        std::vector<VkCommandBuffer>& getBuffers() { return this->commandBuffers; }

        void submitToRender(const SubmitToRenderInfo& sub);

      private:
        VkDevice device;
        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;
    };

    void SubmitQueue(VkQueue queue, VkCommandBuffer commandBuffer);

    void CopyBuffer(VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool, VkBuffer srcBuffer,
                    VkBuffer dstBuffer, VkDeviceSize bufferSize);

    void CopyImageBuffer(VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool, VkBuffer srcBuffer,
                         VkImage image, uint32_t width, uint32_t height);

    void TransitionImageLayout(VkDevice device, VkQueue queue, VkCommandPool commandPool, VkImage image,
                               VkImageLayout oldLayout, VkImageLayout newLayout);

} // namespace ce
#endif
