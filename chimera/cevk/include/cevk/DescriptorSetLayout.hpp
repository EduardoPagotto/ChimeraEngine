#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {
    class DescriptorSetLayout {
      public:
        explicit DescriptorSetLayout() = default;
        virtual ~DescriptorSetLayout() { destroy(); }

        DescriptorSetLayout(const DescriptorSetLayout&) = delete;
        DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;

        void init(VkDevice device) { device_ = device; }
        void destroy();
        void create(void* extended_info = VK_NULL_HANDLE, const VkDescriptorSetLayoutCreateFlags& flags = 0);

        void add_binding(const VkDescriptorSetLayoutBinding& vp_layout_binding) {
            layout_binding_.push_back(vp_layout_binding);
        }

        VkDescriptorSetLayout& get() { return handle_; }

      private:
        VkDevice device_{VK_NULL_HANDLE};
        VkDescriptorSetLayout handle_{VK_NULL_HANDLE};
        std::vector<VkDescriptorSetLayoutBinding> layout_binding_;
    };
} // namespace ce
