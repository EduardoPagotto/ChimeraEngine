#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {
    class DescriptorSet {
      public:
        explicit DescriptorSet() = default;
        virtual ~DescriptorSet() = default;

        void init(VkDevice device) { this->device = device; }

        DescriptorSet(const DescriptorSet&) = delete;
        DescriptorSet& operator=(const DescriptorSet&) = delete;
        // DescriptorSet(DescriptorSet&& other) noexcept;
        // DescriptorSet& operator=(DescriptorSet&& other) noexcept;

        std::pair<size_t, size_t> allocate(const VkDescriptorPool& descriptorPool,
                                           std::vector<VkDescriptorSetLayout>& descriptorSetLayouts);

        std::vector<VkDescriptorSet>& get() { return this->descriptorSets; }

      private:
        VkDevice device{VK_NULL_HANDLE};
        std::vector<VkDescriptorSet> descriptorSets;
    };
} // namespace ce
