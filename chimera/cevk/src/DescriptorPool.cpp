#include "DescriptorPool.hpp"
#include <stdexcept>

namespace ce {

    void DescriptorPool::create(VkDevice device, const uint32_t& max_sets, VkDescriptorPoolCreateFlagBits flags) {

        device_ = device;

        // Data to create Descriptor Pool
        const VkDescriptorPoolCreateInfo pool_create_info{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .flags = flags,
            .maxSets = max_sets, // Maximum number of descriptor Sets that can be create from pool
            .poolSizeCount = static_cast<uint32_t>(pool_size_.size()), // Amount of Pool Sizes being passed
            .pPoolSizes = pool_size_.data()                            // Pool Sizes to create pool with
        };

        if (vkCreateDescriptorPool(device, &pool_create_info, nullptr, &handle_) !=
            VK_SUCCESS) { // Create Descriptor Pool
            throw std::runtime_error("Failed to create Descriptor pool");
        }

        pool_size_.clear();
        pool_size_.shrink_to_fit();
    }

    void DescriptorPool::destroy() noexcept {
        if (handle_ != VK_NULL_HANDLE && device_ != VK_NULL_HANDLE) {
            vkDestroyDescriptorPool(device_, handle_, nullptr);
            handle_ = VK_NULL_HANDLE;
        }
    }
} // namespace ce
