#include "Sync.hpp"
#include <stdexcept>

namespace ce {
    Sync::Sync(VkDevice logical) { this->init(logical); }

    Sync::~Sync() { this->destroy(); }

    void Sync::init(VkDevice logical) {

        this->logical = logical;
        // Semaphore creation information
        const VkSemaphoreCreateInfo semaphoreCreateInfo{
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        };

        // Fence creation information
        const VkFenceCreateInfo fenceCreateInfo{.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                                                .flags = VK_FENCE_CREATE_SIGNALED_BIT};

        if (vkCreateSemaphore(logical, &semaphoreCreateInfo, nullptr, &this->imageAvailable) != VK_SUCCESS ||
            vkCreateSemaphore(logical, &semaphoreCreateInfo, nullptr, &this->renderFinished) != VK_SUCCESS ||
            vkCreateFence(logical, &fenceCreateInfo, nullptr, &this->drawFences) != VK_SUCCESS) {

            throw std::runtime_error("Failed to create a Semaphore and/or Fence!");
        }
    }

    void Sync::destroy() {
        if (this->logical != VK_NULL_HANDLE) {
            vkDestroySemaphore(this->logical, this->renderFinished, nullptr);
            vkDestroySemaphore(this->logical, this->imageAvailable, nullptr);
            vkDestroyFence(this->logical, this->drawFences, nullptr);
            this->logical = VK_NULL_HANDLE;
        }
    }

    void Sync::waitAndResetFence() {
        // Wait for given fence to signal (open) from last draw before continuing
        vkWaitForFences(this->logical, 1, &this->drawFences, VK_TRUE, std::numeric_limits<uint64_t>::max());
        // Manually reset (close) fence
        vkResetFences(this->logical, 1, &this->drawFences);
    }

} // namespace ce
