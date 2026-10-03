#pragma once

#include <vulkan/vulkan_core.h>

namespace ce {

    class CmdBuffer {
      public:
        explicit CmdBuffer() = default;
        explicit CmdBuffer(VkDevice device, VkCommandPool commandpool);
        virtual ~CmdBuffer();

        void init(VkDevice device, VkCommandPool commandpool);
        void destroy();
        void clean();
        void begin(VkCommandBufferUsageFlagBits flag);
        void end();
        void submit_queue(VkQueue queue);

        VkCommandBuffer& get() { return this->handle_; }

      private:
        VkDevice device_{VK_NULL_HANDLE};
        VkCommandPool commandpool_{VK_NULL_HANDLE};
        VkCommandBuffer handle_{VK_NULL_HANDLE};
    };

    namespace aux {

        void CopyBuffer(VkDevice device, VkQueue queue, VkCommandPool commandpool, VkBuffer src_buffer,
                        VkBuffer dst_buffer, VkDeviceSize buffer_size);

        void CopyImageBuffer(VkDevice device, VkQueue queue, VkCommandPool commandpool, VkBuffer src_buffer,
                             VkImage image, uint32_t width, uint32_t height);

        void TransitionImageLayout(VkDevice device, VkQueue queue, VkCommandPool commandpool, VkImage image,
                                   VkImageLayout old_layout, VkImageLayout new_layout);
    } // namespace aux

} // namespace ce
