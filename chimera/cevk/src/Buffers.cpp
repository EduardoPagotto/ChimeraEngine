#include "Buffers.hpp"
#include "VulkanContext.hpp"
#include <cstring>
#include <stdexcept>

namespace ce {

#pragma region Buffer

    void Buffer::init(VkPhysicalDevice physical, VkDevice device) {
        physical_ = physical;
        device_ = device;
    }

    void Buffer::create(const VkDeviceSize& buffer_size, const VkBufferUsageFlags& buffer_usage,
                        const VkMemoryPropertyFlags& buffer_properties) {

        // information to create a buffer (dosen't include assigning memory)
        const VkBufferCreateInfo buffer_info{
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = buffer_size,                      // Size of buffer (size of 1 vertex * number of vertices)
            .usage = buffer_usage,                    // Multiple types of buffer possible
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE, // Similar to Swap Chain images, can share vertex buffers
        };

        buffer_size_ = buffer_size;

        if (vkCreateBuffer(device_, &buffer_info, nullptr, &buffer_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a Buffer!");
        }

        // GET BUFFER MEMORY REQUIREMENTS
        VkMemoryRequirements mem_requirements;
        vkGetBufferMemoryRequirements(device_, buffer_, &mem_requirements);

        // ALLOCATE MEMORY TO BUFFER
        // VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT : CPU can interact with memory
        // VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : Allows placement of data straight into buffer mapping (otherwise would
        // have to specify manually)
        const VkMemoryAllocateInfo memory_alloc_info{
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = mem_requirements.size,
            .memoryTypeIndex = VulkanContext::find_memory_type_index(physical_, mem_requirements.memoryTypeBits,
                                                                     buffer_properties)};

        // Allocate memory to VkDebviceMemory
        if (vkAllocateMemory(device_, &memory_alloc_info, nullptr, &memory_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate Vertex Buffer Memory!!");
        }

        // Allocate memory to given vertex buffer
        vkBindBufferMemory(device_, buffer_, memory_, 0);
    }

    void Buffer::mapper(void* src) {
        // MAP MEMORY TO BUFFER
        // 1. Get and create pointer to point in normal memory
        // 2. "Map" the vertex buffer memory to that point
        // 3. Copy memory from vertices vector to the point
        // 4. Unmap the vertex buffer memory
        void* mapped_data;
        vkMapMemory(device_, memory_, 0, buffer_size_, 0, &mapped_data);
        std::memcpy(mapped_data, static_cast<const void*>(src), static_cast<size_t>(buffer_size_));
        vkUnmapMemory(device_, memory_);
    }

    void Buffer::destroy() {
        if (device_ == VK_NULL_HANDLE) {
            return;
        }

        // Se a memória estava mapeada, desmapeia primeiro
        // if ((mapped_data_ != nullptr) && (memory != VK_NULL_HANDLE)) {
        //     vkUnmapMemory(device, memory);
        //     mapped_data_ = nullptr;
        // }

        // Deleta o buffer se ele existir
        if (buffer_ != VK_NULL_HANDLE) {
            vkDestroyBuffer(device_, buffer_, nullptr);
            buffer_ = VK_NULL_HANDLE;
        }

        // Libera a memória alocada por último
        if (memory_ != VK_NULL_HANDLE) {
            vkFreeMemory(device_, memory_, nullptr);
            memory_ = VK_NULL_HANDLE;
        }

        device_ = VK_NULL_HANDLE;
    }
} // namespace ce
