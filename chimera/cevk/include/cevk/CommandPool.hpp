#pragma once

#include <vulkan/vulkan_core.h>

namespace ce {

    class CommandPool {
      public:
        explicit CommandPool() = default;
        explicit CommandPool(VkDevice logical, uint32_t queueGraphicsFamilyIndex);

        CommandPool(const CommandPool&) = delete;
        CommandPool& operator=(const CommandPool&) = delete;

        virtual ~CommandPool();

        void init(VkDevice logical, uint32_t queueGraphicsFamilyIndex);
        void destroy();
        void cleanup();

        VkCommandPool& get() { return this->commandPool; }

      private:
        VkDevice logical{VK_NULL_HANDLE};
        VkCommandPool commandPool{VK_NULL_HANDLE};
    };
} // namespace ce
