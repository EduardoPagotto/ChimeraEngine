#pragma once

#include <vulkan/vulkan_core.h>

namespace ce {

    class CommandPool {
      public:
        explicit CommandPool(VkDevice logical, uint32_t queueGraphicsFamilyIndex);
        virtual ~CommandPool();
        void cleanup();
        VkCommandPool& get() { return this->commandPool; }

      private:
        VkDevice logical{VK_NULL_HANDLE};
        VkCommandPool commandPool{VK_NULL_HANDLE};
    };
} // namespace ce
