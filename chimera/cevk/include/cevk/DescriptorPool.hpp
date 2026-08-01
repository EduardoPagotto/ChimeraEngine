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

        VkDescriptorPool& get() { return handle; }

        void addPoolSize(const VkDescriptorPoolSize& poolsize) { this->poolSize.push_back(poolsize); }

        void create(VkDevice device, const uint32_t& maxSets, VkDescriptorPoolCreateFlagBits flags);
        void destroy() noexcept;

      private:
        VkDevice device{VK_NULL_HANDLE};
        VkDescriptorPool handle{VK_NULL_HANDLE};
        std::vector<VkDescriptorPoolSize> poolSize;
    };
} // namespace ce
