#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {
    class DescriptorPool {
      public:
        explicit DescriptorPool(VkDevice device) : device(device) {}
        virtual ~DescriptorPool() { cleanup(); }

        DescriptorPool(const DescriptorPool&) = delete;
        DescriptorPool& operator=(const DescriptorPool&) = delete;
        // DescriptorPool(DescriptorPool&& other) noexcept;
        // DescriptorPool& operator=(DescriptorPool&& other) noexcept;
        // explicit operator VkDescriptorPool() const noexcept;

        [[nodiscard]] VkDescriptorPool& get() { return handle; }

        void addPoolSize(const VkDescriptorType& type, const uint32_t& count);
        void create(const uint32_t& maxSets);
        void cleanup() noexcept;

      private:
        VkDevice device{VK_NULL_HANDLE};
        VkDescriptorPool handle{VK_NULL_HANDLE};
        std::vector<VkDescriptorPoolSize> poolSize;
    };
} // namespace ce
