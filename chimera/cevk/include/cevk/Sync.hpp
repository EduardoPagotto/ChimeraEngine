#ifndef __SYNC_CLASS_HPP
#define __SYNC_CLASS_HPP

#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {

    class Sync {
      public:
        explicit Sync(VkDevice logical, size_t max);
        virtual ~Sync();

        void waitAndResetFence(size_t index);

        VkSemaphore& getWaitSemafore(size_t index) { return this->imageAvailable[index]; }
        VkSemaphore& getSignalSemaphore(size_t index) { return this->renderFinished[index]; }
        VkFence& getDrawFence(size_t index) { return this->drawFences[index]; }

      private:
        VkDevice logical;
        size_t max;

        std::vector<VkSemaphore> imageAvailable;
        std::vector<VkSemaphore> renderFinished;
        std::vector<VkFence> drawFences;
    };

} // namespace ce
#endif
