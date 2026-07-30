#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {
    class DescriptorSet {
      public:
        explicit DescriptorSet() = default;
        virtual ~DescriptorSet() = default;

        void init(VkDevice device) { this->device = device; }

        VkDescriptorSet& get() { return this->descriptorSets; }

        void alloc(const VkDescriptorPool& descriptorPool, VkDescriptorSetLayout& descriptorSetLayouts,
                   void* variableCountInfo = VK_NULL_HANDLE);

      private:
        VkDevice device{VK_NULL_HANDLE};
        VkDescriptorSet descriptorSets{VK_NULL_HANDLE};
    };

    class DescriptorSetWrite {
      public:
        DescriptorSetWrite(VkDevice device) : device(device) {}
        virtual ~DescriptorSetWrite() = default;

        void update() {
            // Update the descripto sets with new buffer/binding info
            vkUpdateDescriptorSets(device, static_cast<uint32_t>(this->setWrites.size()), this->setWrites.data(), 0,
                                   nullptr);

            this->setWrites.clear();
            this->setWrites.shrink_to_fit();
        }

        void add(const VkWriteDescriptorSet& vpSetWrite) { this->setWrites.push_back(vpSetWrite); }

      private:
        VkDevice device{VK_NULL_HANDLE};
        std::vector<VkWriteDescriptorSet> setWrites;
    };
} // namespace ce
