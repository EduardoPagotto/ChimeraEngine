#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {
    class DescriptorPool {
      public:
        explicit DescriptorPool() = default;
        virtual ~DescriptorPool() noexcept { destroy(); }

        DescriptorPool(const DescriptorPool&) = delete;
        DescriptorPool& operator=(const DescriptorPool&) = delete;

        VkDescriptorPool& get() { return handle_; }

        void add_pool_size(const VkDescriptorPoolSize& poolsize) { pool_size_.push_back(poolsize); }

        void create(VkDevice device, const uint32_t& max_sets, VkDescriptorPoolCreateFlagBits flags);
        void destroy() noexcept;

      private:
        VkDevice device_{VK_NULL_HANDLE};
        VkDescriptorPool handle_{VK_NULL_HANDLE};
        std::vector<VkDescriptorPoolSize> pool_size_;
    };
} // namespace ce
