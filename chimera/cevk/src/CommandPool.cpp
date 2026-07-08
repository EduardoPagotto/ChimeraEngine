#include "CommandPool.hpp"
#include "DevVK.hpp"
#include <stdexcept>

namespace ce {
    //
    CommandPool::CommandPool(std::shared_ptr<BaseVK> bvk) : bvk(bvk) { // NOLINT

        // Get inidices of queue families from device
        QueueFamilyIndices queueFamilyIndices = aux::GetQueueFamilies(bvk->physical, bvk->surface);

        const VkCommandPoolCreateInfo poolInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex =
                static_cast<uint32_t>(queueFamilyIndices.graphicsFamily) // Queue Family type that buffers from this command pool will use
        };

        // Create a Graphics Queue Family Command Pool
        if (vkCreateCommandPool(bvk->logical, &poolInfo, nullptr, &this->commandPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Command Pool");
        }
    }

    CommandPool::~CommandPool() {
        //
        vkDestroyCommandPool(this->bvk->logical, this->commandPool, nullptr);
    }

    void CommandPool::cleanup() {
        //
        if (vkResetCommandPool(this->bvk->logical, this->commandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT) != VK_SUCCESS) {
            throw std::runtime_error("Failed to Reset Command Pool");
        }
    }
} // namespace ce
