#include "DescriptorSet.hpp"
#include <stdexcept>

namespace ce {
    std::pair<size_t, size_t> DescriptorSet::allocate(const VkDescriptorPool& descriptorPool,
                                                      std::vector<VkDescriptorSetLayout>& descriptorSetLayouts,
                                                      void* variableCountInfo) {

        // Reserve new spaces in descriptorSet
        size_t index = this->descriptorSets.size();
        size_t size = descriptorSetLayouts.size(); // Total of descriptors to allocate size

        // Reserve space to temprary desciptor
        std::vector<VkDescriptorSet> localSets(size);

        // Descriptor Set Allocation info
        const VkDescriptorSetAllocateInfo setAllocInfo{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext = variableCountInfo,
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

} // namespace ce
