#pragma once

#include <vulkan/vulkan_core.h>

namespace ce {

    class CmdPool {
      public:
        explicit CmdPool() = default;
        explicit CmdPool(VkDevice logical, uint32_t queueGraphicsFamilyIndex);

        CmdPool(const CmdPool&) = delete;
        CmdPool& operator=(const CmdPool&) = delete;

        virtual ~CmdPool();

        void init(VkDevice logical, uint32_t queueGraphicsFamilyIndex);
        void destroy();
        void cleanup();

        VkCommandPool& get() { return this->commandPool; }

      private:
        VkDevice logical{VK_NULL_HANDLE};
        VkCommandPool commandPool{VK_NULL_HANDLE};
    };
} // namespace ce
