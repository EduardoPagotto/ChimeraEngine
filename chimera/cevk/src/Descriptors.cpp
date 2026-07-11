#include "Descriptors.hpp"
#include <stdexcept>

namespace ce {

#pragma region DescriptorPool

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

    void DescriptorPool::cleanup() noexcept {
        if (this->handle != VK_NULL_HANDLE && this->device != VK_NULL_HANDLE) {
            vkDestroyDescriptorPool(this->device, this->handle, nullptr);
            handle = VK_NULL_HANDLE;
        }
    }

#pragma endregion

#pragma region DescriptorSet

    std::pair<size_t, size_t> DescriptorSet::allocate(const VkDescriptorPool& descriptorPool,
                                                      std::vector<VkDescriptorSetLayout>& descriptorSetLayouts) {

        // Reserve new spaces in descriptorSet
        size_t index = this->descriptorSets.size();
        size_t size = descriptorSetLayouts.size(); // Total of descriptors to allocate size

        // Reserve space to temprary desciptor
        std::vector<VkDescriptorSet> localSets(size);

        // Descriptor Set Allocation info
        const VkDescriptorSetAllocateInfo setAllocInfo{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorPool = descriptorPool,                  // Pool to allocate Descriptor Set
            .descriptorSetCount = static_cast<uint32_t>(size), // Number of sets to allocate
            .pSetLayouts = descriptorSetLayouts.data()         // Layouts to use to allocate sets (1:1 relationship)
        };

        // Allocate descriptor sets (multiple)
        if (vkAllocateDescriptorSets(device, &setAllocInfo, localSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate DescriptorSet");
        }

        // copy localSets sets to descriptorSets
        this->descriptorSets.insert(this->descriptorSets.end(), localSets.begin(), localSets.end());

        return {index, size}; // start position, total new allocate
    }

#pragma endregion

#pragma region DescriptorSetLayout

    DescriptorSetLayout::~DescriptorSetLayout() {
        if (handle != VK_NULL_HANDLE && device != VK_NULL_HANDLE) {
            vkDestroyDescriptorSetLayout(device, this->handle, nullptr);
        }
    }

    void DescriptorSetLayout::create() {

        // Create Desciptor Set Layout with given bindingd
        const VkDescriptorSetLayoutCreateInfo layoutCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .bindingCount = static_cast<uint32_t>(layoutBinding.size()), // Number of binding infos
            .pBindings = layoutBinding.data()                            // Array of binding infos
        };

        // Create Descriptor Set Layout
        if (vkCreateDescriptorSetLayout(this->device, &layoutCreateInfo, nullptr, &this->handle) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create descriptor set Layout!");
        }

        layoutBinding.clear();
        layoutBinding.shrink_to_fit();
    }
#pragma endregion

} // namespace ce
