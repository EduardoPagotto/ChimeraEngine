#include "Buffers.hpp"
#include "cevk.hpp"
#include <cstring>
#include <stdexcept>

namespace ce {

#pragma region Buffer

    void Buffer::init(VkPhysicalDevice physical, VkDevice device) {
        this->physical = physical;
        this->device = device;
    }

    void Buffer::create(const VkDeviceSize& bufferSize, const VkBufferUsageFlags& bufferUsage,
                        const VkMemoryPropertyFlags& bufferProperties) {

        // information to create a buffer (dosen't include assigning memory)
        const VkBufferCreateInfo bufferInfo{
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = bufferSize,                       // Size of buffer (size of 1 vertex * number of vertices)
            .usage = bufferUsage,                     // Multiple types of buffer possible
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE, // Similar to Swap Chain images, can share vertex buffers
        };

        this->bufferSize = bufferSize;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &this->buffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a Buffer!");
        }

        // GET BUFFER MEMORY REQUIREMENTS
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, this->buffer, &memRequirements);

        // ALLOCATE MEMORY TO BUFFER
        // VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT : CPU can interact with memory
        // VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : Allows placement of data straight into buffer mapping (otherwise would
        // have to specify manually)
        const VkMemoryAllocateInfo memoryAllocInfo{
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memRequirements.size,
            .memoryTypeIndex =
                VulkanContext::FindMemoryTypeIndex(this->physical, memRequirements.memoryTypeBits, bufferProperties)};

        // Allocate memory to VkDebviceMemory
        if (vkAllocateMemory(this->device, &memoryAllocInfo, nullptr, &this->memory) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate Vertex Buffer Memory!!");
        }

        // Allocate memory to given vertex buffer
        vkBindBufferMemory(this->device, this->buffer, this->memory, 0);
    }

    void Buffer::mapper(void* src) {
        // MAP MEMORY TO BUFFER
        // 1. Get and create pointer to point in normal memory
        // 2. "Map" the vertex buffer memory to that point
        // 3. Copy memory from vertices vector to the point
        // 4. Unmap the vertex buffer memory
        void* mappedData;
        vkMapMemory(this->device, this->memory, 0, this->bufferSize, 0, &mappedData);
        std::memcpy(mappedData, static_cast<const void*>(src), static_cast<size_t>(bufferSize));
        vkUnmapMemory(device, memory);
    }

    void Buffer::destroy() {
        if (device == VK_NULL_HANDLE) {
            return;
        }

        // Se a memória estava mapeada, desmapeia primeiro
        // if ((mappedData != nullptr) && (memory != VK_NULL_HANDLE)) {
        //     vkUnmapMemory(device, memory);
        //     mappedData = nullptr;
        // }

        // Deleta o buffer se ele existir
        if (buffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(device, buffer, nullptr);
            buffer = VK_NULL_HANDLE;
        }

        // Libera a memória alocada por último
        if (memory != VK_NULL_HANDLE) {
            vkFreeMemory(device, memory, nullptr);
            memory = VK_NULL_HANDLE;
        }

        device = VK_NULL_HANDLE;
    }
} // namespace ce
