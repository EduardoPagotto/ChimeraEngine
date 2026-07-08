#pragma once

#include "subsystem.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>

namespace ce {

    class CommandPool {
      public:
        explicit CommandPool(std::shared_ptr<BaseVK> bvk);
        virtual ~CommandPool();
        void cleanup();
        VkCommandPool& getPool() { return this->commandPool; }

      private:
        std::shared_ptr<BaseVK> bvk;
        VkCommandPool commandPool;
    };
} // namespace ce
