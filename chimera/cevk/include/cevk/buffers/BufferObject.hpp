#ifndef __BUFFEROBJECT_CLASS_HPP
#define __BUFFEROBJECT_CLASS_HPP

#include <vulkan/vulkan_core.h>

namespace ce {

    class BufferObject {
      public:
        explicit BufferObject(VkPhysicalDevice physical, VkDevice device) : physical(physical), device(device) {}
        virtual ~BufferObject();

        // Proíbe cópia para evitar dupla desalocação
        BufferObject(const BufferObject&) = delete;
        BufferObject& operator=(const BufferObject&) = delete;
        // BufferObject(BufferObject&& other) noexcept;
        // BufferObject& operator=(BufferObject&& other) noexcept;

        void create(const VkDeviceSize& bufferSize, const VkBufferUsageFlags& bufferUsage,
                    const VkMemoryPropertyFlags& bufferProperties);

        [[nodiscard]] VkBuffer getBuffer() const { return buffer; }
        [[nodiscard]] VkDeviceMemory getMemory() const { return memory; }
        [[nodiscard]] bool isValid() const { return memory != VK_NULL_HANDLE; }
        //[[nodiscard]] void* getMappedData() const { return mappedData; }

        void mapper(void* src);

      private:
        void destroy();

        VkPhysicalDevice physical{VK_NULL_HANDLE};
        VkDevice device{VK_NULL_HANDLE};
        VkBuffer buffer{VK_NULL_HANDLE};
        VkDeviceMemory memory{VK_NULL_HANDLE};
        VkDeviceSize bufferSize;
        // void* mappedData{nullptr};
    };
} // namespace ce
#endif
