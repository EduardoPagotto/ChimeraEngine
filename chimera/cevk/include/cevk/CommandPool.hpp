#pragma once

#include "cevk.hpp"

namespace ce {

    class CommandPool {
      public:
        explicit CommandPool(BaseVK* pBVK);
        virtual ~CommandPool();
        void cleanup();
        VkCommandPool& get() { return this->commandPool; }

      private:
        VkDevice logical{VK_NULL_HANDLE};
        VkCommandPool commandPool;
    };
} // namespace ce
