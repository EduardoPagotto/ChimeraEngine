#include "DescriptorPool.hpp"
#include <stdexcept>

namespace ce {

    void DescriptorPool::create(VkDevice device, const uint32_t& maxSets, VkDescriptorPoolCreateFlagBits flags) {

        this->device = device;
        this->flags = flags;

        // Data to create Descriptor Pool
        const VkDescriptorPoolCreateInfo poolCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .flags = flags,
            .maxSets = maxSets, // Maximum number of descriptor Sets that can be create from pool
            .poolSizeCount = static_cast<uint32_t>(this->poolSize.size()), // Amount of Pool Sizes being passed
            .pPoolSizes = this->poolSize.data()                            // Pool Sizes to create pool with
        };

        if (vkCreateDescriptorPool(device, &poolCreateInfo, nullptr, &this->handle) !=
            VK_SUCCESS) { // Create Descriptor Pool
            throw std::runtime_error("Failed to create Descriptor pool");
        }

        this->poolSize.clear();
        this->poolSize.shrink_to_fit();
    }

    void DescriptorPool::destroy() noexcept {
        if (this->handle != VK_NULL_HANDLE && this->device != VK_NULL_HANDLE) {
            vkDestroyDescriptorPool(this->device, this->handle, nullptr);
            handle = VK_NULL_HANDLE;
        }
    }

} // namespace ce
