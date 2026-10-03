#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {
    class DescriptorSet {
      public:
        explicit DescriptorSet() = default;
        virtual ~DescriptorSet() = default;

        void init(VkDevice device) { this->device_ = device; }

        VkDescriptorSet& get() { return this->descriptor_sets_; }

        void alloc(const VkDescriptorPool& descriptor_pool, VkDescriptorSetLayout& descriptor_set_layouts,
                   void* variable_count_info = VK_NULL_HANDLE);

      private:
        VkDevice device_{VK_NULL_HANDLE};
        VkDescriptorSet descriptor_sets_{VK_NULL_HANDLE};
    };

    class DescriptorSetWrite {
      public:
        DescriptorSetWrite(VkDevice device) : device_(device) {}
        virtual ~DescriptorSetWrite() = default;

        void update() {
            // Update the descripto sets with new buffer/binding info
            vkUpdateDescriptorSets(device_, static_cast<uint32_t>(this->set_writes_.size()), this->set_writes_.data(),
                                   0, nullptr);

            this->set_writes_.clear();
            this->set_writes_.shrink_to_fit();
        }

        void add(const VkWriteDescriptorSet& vp_set_write) { this->set_writes_.push_back(vp_set_write); }

      private:
        VkDevice device_{VK_NULL_HANDLE};
        std::vector<VkWriteDescriptorSet> set_writes_;
    };
} // namespace ce
