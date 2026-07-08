#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {

    class DescriptorSetLayout {
      public:
        explicit DescriptorSetLayout(VkDevice device) : device(device) {}
        virtual ~DescriptorSetLayout();

        DescriptorSetLayout(const DescriptorSetLayout&) = delete;
        DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;
        // DescriptorSetLayout(DescriptorSetLayout&& other) noexcept;
        // DescriptorSetLayout& operator=(DescriptorSetLayout&& other) noexcept;

        void create();

        void addBinding(const VkDescriptorSetLayoutBinding& vpLayoutBinding) { this->layoutBinding.push_back(vpLayoutBinding); }
        [[nodiscard]] VkDescriptorSetLayout& get() { return this->handle; }

      private:
        VkDevice device{VK_NULL_HANDLE};
        VkDescriptorSetLayout handle{VK_NULL_HANDLE};
        std::vector<VkDescriptorSetLayoutBinding> layoutBinding;
    };
} // namespace ce
