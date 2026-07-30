#include "DescriptorSet.hpp"
#include <stdexcept>

namespace ce {
    void DescriptorSet::alloc(const VkDescriptorPool& descriptorPool, VkDescriptorSetLayout& descriptorSetLayouts,
                              void* variableCountInfo) {

        // Descriptor Set Allocation info
        const VkDescriptorSetAllocateInfo setAllocInfo{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext = variableCountInfo,
            .descriptorPool = descriptorPool,               // Pool to allocate Descriptor Set
            .descriptorSetCount = static_cast<uint32_t>(1), // Number of sets to allocate
            .pSetLayouts = &descriptorSetLayouts};

        // Allocate descriptor set
        if (vkAllocateDescriptorSets(device, &setAllocInfo, &descriptorSets) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate DescriptorSet");
        }
    }
} // namespace ce
