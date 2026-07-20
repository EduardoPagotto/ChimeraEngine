#pragma once

#include "Sync.hpp"
#include <vulkan/vulkan_core.h>

namespace ce {

    class CommandBuffer {
      public:
        explicit CommandBuffer() = default;
        explicit CommandBuffer(VkDevice device, VkCommandPool commandPool);
        virtual ~CommandBuffer();

        void init(VkDevice device, VkCommandPool commandPool);
        void destroy();
        void clean();
        void begin(VkCommandBufferUsageFlagBits flag);
        void end();

        void submitToRender(VkQueue queue, Sync& sync, const VkPipelineStageFlagBits& pipelineStageFlags);
        void submitQueue(VkQueue queue);

        VkCommandBuffer& get() { return this->handle; }

      private:
        VkDevice device{VK_NULL_HANDLE};
        VkCommandPool commandPool{VK_NULL_HANDLE};
        VkCommandBuffer handle{VK_NULL_HANDLE};
    };

    namespace aux {

        void CopyBuffer(VkDevice device, VkQueue queue, VkCommandPool commandPool, VkBuffer srcBuffer,
                        VkBuffer dstBuffer, VkDeviceSize bufferSize);

        void CopyImageBuffer(VkDevice device, VkQueue queue, VkCommandPool commandPool, VkBuffer srcBuffer,
                             VkImage image, uint32_t width, uint32_t height);

        void TransitionImageLayout(VkDevice device, VkQueue queue, VkCommandPool commandPool, VkImage image,
                                   VkImageLayout oldLayout, VkImageLayout newLayout);
    } // namespace aux

} // namespace ce
