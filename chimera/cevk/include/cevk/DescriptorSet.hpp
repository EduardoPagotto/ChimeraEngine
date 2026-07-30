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
                                           std::vector<VkDescriptorSetLayout>& descriptorSetLayouts,
                                           void* variableCountInfo = VK_NULL_HANDLE);

        VkDescriptorSet& get(size_t index) { return this->descriptorSets[index]; }

        size_t getSize() const { return this->descriptorSets.size(); }

        void update() {
            // Update the descripto sets with new buffer/binding info
            vkUpdateDescriptorSets(device, static_cast<uint32_t>(this->setWrites.size()), this->setWrites.data(), 0,
                                   nullptr);
        }

        void addWrite(const VkWriteDescriptorSet& vpSetWrite) { this->setWrites.push_back(vpSetWrite); }
        void clearWrite() { this->setWrites.clear(); }

      private:
        VkDevice device{VK_NULL_HANDLE};
        std::vector<VkDescriptorSet> descriptorSets;
        std::vector<VkWriteDescriptorSet> setWrites;
    };
} // namespace ce
