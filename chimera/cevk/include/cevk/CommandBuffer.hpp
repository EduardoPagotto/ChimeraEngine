#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {
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

      private:
        VkDevice device;
        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;
    };

    namespace aux {

        void SubmitQueue(VkQueue queue, VkCommandBuffer commandBuffer);

        void CopyBuffer(VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool, VkBuffer srcBuffer,
                        VkBuffer dstBuffer, VkDeviceSize bufferSize);

        void CopyImageBuffer(VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool,
                             VkBuffer srcBuffer, VkImage image, uint32_t width, uint32_t height);

        void TransitionImageLayout(VkDevice device, VkQueue queue, VkCommandPool commandPool, VkImage image,
                                   VkImageLayout oldLayout, VkImageLayout newLayout);
    } // namespace aux

} // namespace ce
