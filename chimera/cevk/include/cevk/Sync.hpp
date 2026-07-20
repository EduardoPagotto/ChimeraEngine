#pragma once

#include <vulkan/vulkan_core.h>

namespace ce {

    class Sync {
      public:
        explicit Sync() = default;
        explicit Sync(VkDevice logical);
        virtual ~Sync();

        void init(VkDevice logical);
        void destroy();

        void waitAndResetFence();
        VkSemaphore& getWait() { return this->imageAvailable; }
        VkSemaphore& getSignal() { return this->renderFinished; }
        VkFence& getFence() { return this->drawFences; }

      private:
        VkDevice logical{VK_NULL_HANDLE};
        VkSemaphore imageAvailable{VK_NULL_HANDLE};
        VkSemaphore renderFinished{VK_NULL_HANDLE};
        VkFence drawFences{VK_NULL_HANDLE};
    };

} // namespace ce
