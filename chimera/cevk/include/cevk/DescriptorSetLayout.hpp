#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {
    class DescriptorSetLayout {
      public:
        explicit DescriptorSetLayout() = default;
        virtual ~DescriptorSetLayout() { this->destroy(); }

        DescriptorSetLayout(const DescriptorSetLayout&) = delete;
        DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;

        void init(VkDevice device) { this->device = device; }
        void destroy();
        void create();

        void addBinding(const VkDescriptorSetLayoutBinding& vpLayoutBinding) {
            this->layoutBinding.push_back(vpLayoutBinding);
        }

        VkDescriptorSetLayout& get() { return this->handle; }

      private:
        VkDevice device{VK_NULL_HANDLE};
        VkDescriptorSetLayout handle{VK_NULL_HANDLE};
        std::vector<VkDescriptorSetLayoutBinding> layoutBinding;
    };
} // namespace ce
