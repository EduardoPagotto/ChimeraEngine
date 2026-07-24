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

        void addPoolSize(const VkDescriptorType& type, const uint32_t& count) {
            this->poolSize.push_back(VkDescriptorPoolSize{.type = type, .descriptorCount = count});
        }

        void create(VkDevice device, const uint32_t& maxSets, VkDescriptorPoolCreateFlagBits flags);
        void destroy() noexcept;

        VkDescriptorPoolCreateFlagBits getFlags() { return this->flags; }

      private:
        VkDevice device{VK_NULL_HANDLE};
        VkDescriptorPool handle{VK_NULL_HANDLE};
        VkDescriptorPoolCreateFlagBits flags;
        std::vector<VkDescriptorPoolSize> poolSize;
    };
} // namespace ce
