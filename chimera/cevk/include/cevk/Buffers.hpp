#pragma once

#include <vulkan/vulkan_core.h>

namespace ce {

    class Buffer {
      public:
        explicit Buffer() = default;
        explicit Buffer(VkPhysicalDevice physical, VkDevice device) { this->init(physical, device); }
        virtual ~Buffer() { this->destroy(); }

        void init(VkPhysicalDevice physical, VkDevice device);

        void create(const VkDeviceSize& bufferSize, const VkBufferUsageFlags& bufferUsage,
                    const VkMemoryPropertyFlags& bufferProperties);

        VkBuffer get() const { return buffer; }
        VkDeviceMemory getMemory() const { return memory; }
        bool isValid() const { return memory != VK_NULL_HANDLE; }
        //[[nodiscard]] void* getMappedData() const { return mappedData; }

        void mapper(void* src);

        void destroy();

      private:
        VkPhysicalDevice physical{VK_NULL_HANDLE};
        VkDevice device{VK_NULL_HANDLE};
        VkBuffer buffer{VK_NULL_HANDLE};
        VkDeviceMemory memory{VK_NULL_HANDLE};
        VkDeviceSize bufferSize;
        // void* mappedData{nullptr};
    };
} // namespace ce
