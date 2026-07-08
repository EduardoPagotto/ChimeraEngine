#include "descriptors/DescriptorPool.hpp"
#include <stdexcept>

namespace ce {

    // DescriptorPool::DescriptorPool(DescriptorPool&& other) noexcept
    //     : device{std::exchange(other.device, VK_NULL_HANDLE)}, handle{std::exchange(other.handle, VK_NULL_HANDLE)} {}

    // DescriptorPool& DescriptorPool::operator=(DescriptorPool&& other) noexcept {
    //     if (this != &other) {
    //         cleanup(); // Destrói o recurso atual antes de assumir o novo
    //         device = std::exchange(other.device, VK_NULL_HANDLE);
    //         handle = std::exchange(other.handle, VK_NULL_HANDLE);
    //     }
    //     return *this;
    // }

    // DescriptorPool::operator VkDescriptorPool() const noexcept { return handle; }

    void DescriptorPool::addPoolSize(const VkDescriptorType& type, const uint32_t& count) {
        // Type of Descriptors + how many DESCRIPTORS, not Descriptor Sets (combined makes the pool size)
        this->poolSize.push_back(VkDescriptorPoolSize{.type = type, .descriptorCount = count});
    }

    void DescriptorPool::create(const uint32_t& maxSets) {

        // Data to create Descriptor Pool
        const VkDescriptorPoolCreateInfo poolCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .maxSets = maxSets,                                            // Maximum number of descriptor Sets that can be create from pool
            .poolSizeCount = static_cast<uint32_t>(this->poolSize.size()), // Amount of Pool Sizes being passed
            .pPoolSizes = this->poolSize.data()                            // Pool Sizes to create pool with
        };

        if (vkCreateDescriptorPool(device, &poolCreateInfo, nullptr, &this->handle) != VK_SUCCESS) { // Create Descriptor Pool
            throw std::runtime_error("Failed to create Descriptor pool");
        }

        this->poolSize.clear();
        this->poolSize.shrink_to_fit();
    }

    void DescriptorPool::cleanup() noexcept {
        if (this->handle != VK_NULL_HANDLE && this->device != VK_NULL_HANDLE) {
            vkDestroyDescriptorPool(this->device, this->handle, nullptr);
            handle = VK_NULL_HANDLE;
        }
    }
} // namespace ce
