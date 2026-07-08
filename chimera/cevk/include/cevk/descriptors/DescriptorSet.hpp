#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {

    class DescriptorSet {
      public:
        explicit DescriptorSet(VkDevice device) : device(device) {}
        virtual ~DescriptorSet() = default;

        DescriptorSet(const DescriptorSet&) = delete;
        DescriptorSet& operator=(const DescriptorSet&) = delete;
        // DescriptorSet(DescriptorSet&& other) noexcept;
        // DescriptorSet& operator=(DescriptorSet&& other) noexcept;

        std::pair<size_t, size_t> allocate(const VkDescriptorPool& descriptorPool,
                                           std::vector<VkDescriptorSetLayout>& descriptorSetLayouts);

        [[nodiscard]] std::vector<VkDescriptorSet>& get() { return this->descriptorSets; }

      private:
        VkDevice device{VK_NULL_HANDLE};
        std::vector<VkDescriptorSet> descriptorSets;
    };
} // namespace ce
