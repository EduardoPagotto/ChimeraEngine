#pragma once

#include <vulkan/vulkan_core.h>

namespace ce {

#pragma region Buffer

    class Buffer {
      public:
        explicit Buffer(VkPhysicalDevice physical, VkDevice device) : physical(physical), device(device) {}
        virtual ~Buffer();

        // Proíbe cópia para evitar dupla desalocação
        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;
        // Buffer(Buffer&& other) noexcept;
        // Buffer& operator=(Buffer&& other) noexcept;

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

#pragma endregion

#pragma region Image

#pragma endregion

#pragma region CommandBuffer

#pragma endregion

#pragma region BufferDynamic

    // struct UboModel {
    //     glm::mat4 model;
    // };

    // class BufferDynamic { // TODO: validar se precisa existir
    //   public:
    //     explicit BufferDynamic(VkDevice logical) : logical(logical) {}
    //     ~BufferDynamic() = default;

    //   private:
    //     void allocateDynamicBufferTransferSpace() {

    //         // Caculate alignment of model data
    //         this->modelUniformAlignment =
    //             (sizeof(UboModel) + this->minUniformBufferOffset - 1) & ~(this->minUniformBufferOffset - 1);

    //         // Create space in memory to hold dynamic byffer that is alignment and holds MAX_OBJECTS
    //         this->modelTransferSpace =
    //             (UboModel*)aligned_alloc(this->modelUniformAlignment, this->modelUniformAlignment * MAX_OBJECTS);
    //     }

    //     void updateUniformBuffers(uint32_t imageIndex, const std::vector<Mesh>& meshList) {

    //         // Copy Model data
    //         void* data = nullptr;
    //         // for (size_t i = 0; i < meshList.size(); i++) {

    //         //     std::byte* ptr_base = reinterpret_cast<std::byte*>(this->modelTransferSpace);
    //         //     std::byte* ptr_atual = ptr_base + (i * this->modelUniformAlignment);
    //         //     UboModel* thisModel = std::launder(reinterpret_cast<UboModel*>(ptr_atual));

    //         //     *thisModel = meshList[i].getModel(); // FIXME: modelo antigo funcionava, reavaliar
    //         // }

    //         // Map the list of model data // FIXME: usar class Buffer abaixo!!
    //         vkMapMemory(logical, this->modelDUniformBufferMemory[imageIndex], 0,
    //                     this->modelUniformAlignment * meshList.size(), 0, &data);
    //         std::memcpy(data, this->modelTransferSpace, this->modelUniformAlignment * meshList.size());
    //         vkUnmapMemory(logical, this->modelDUniformBufferMemory[imageIndex]);
    //     }

    //     VkDevice logical;

    //     VkDeviceSize minUniformBufferOffset;
    //     size_t modelUniformAlignment;
    //     UboModel* modelTransferSpace;

    //     std::vector<VkBuffer> modelDUniformBuffer;
    //     std::vector<VkDeviceMemory> modelDUniformBufferMemory;
    // };
#pragma endregion
} // namespace ce
