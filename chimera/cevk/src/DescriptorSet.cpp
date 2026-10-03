#include "DescriptorSet.hpp"
#include <stdexcept>

namespace ce {
    void DescriptorSet::alloc(const VkDescriptorPool& descriptor_pool, VkDescriptorSetLayout& descriptor_set_layouts,
                              void* variable_count_info) {

        // Descriptor Set Allocation info
        const VkDescriptorSetAllocateInfo set_alloc_info{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext = variable_count_info,
            .descriptorPool = descriptor_pool,              // Pool to allocate Descriptor Set
            .descriptorSetCount = static_cast<uint32_t>(1), // Number of sets to allocate
            .pSetLayouts = &descriptor_set_layouts};

        // Allocate descriptor set
        if (vkAllocateDescriptorSets(device_, &set_alloc_info, &descriptor_sets_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate DescriptorSet");
        }
    }
} // namespace ce
