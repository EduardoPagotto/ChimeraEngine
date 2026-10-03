#pragma once

#include <vulkan/vulkan_core.h>

namespace ce {

    class Buffer {
      public:
        explicit Buffer() = default;
        explicit Buffer(VkPhysicalDevice physical, VkDevice device) { init(physical, device); }
        virtual ~Buffer() { destroy(); }

        void init(VkPhysicalDevice physical, VkDevice device);

        void create(const VkDeviceSize& buffer_size, const VkBufferUsageFlags& buffer_usage,
                    const VkMemoryPropertyFlags& buffer_properties);

        VkBuffer get() const { return buffer_; }
        VkDeviceMemory get_memory() const { return memory_; }
        bool is_valid() const { return memory_ != VK_NULL_HANDLE; }
        //[[nodiscard]] void* get_mappedData() const { return mapped_data_; }

        void mapper(void* src);

        void destroy();

      private:
        VkPhysicalDevice physical_{VK_NULL_HANDLE};
        VkDevice device_{VK_NULL_HANDLE};
        VkBuffer buffer_{VK_NULL_HANDLE};
        VkDeviceMemory memory_{VK_NULL_HANDLE};
        VkDeviceSize buffer_size_;
        // void* mapped_data_{nullptr};
    };
} // namespace ce
