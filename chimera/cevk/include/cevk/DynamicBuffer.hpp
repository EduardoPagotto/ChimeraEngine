#pragma once

#include <cstdlib>
#include <vector>
#include <vulkan/vulkan_core.h>
namespace ce {

    // struct UboModel {
    //     glm::mat4 model;
    // };

    // TODO: Implementar
    class BufferDynamic {
      public:
        explicit BufferDynamic(VkDevice logical) : logical(logical) {}
        ~BufferDynamic() = default;

      private:
        void* allocateDynamicBufferTransferSpace(size_t size, uint32_t max) { // size:=sizeof(UboModel), MAX_OBJECTS

            // Caculate alignment of model data
            this->uniformAlignment = (size + this->minOffset - 1) & ~(this->minOffset - 1);

            // Create space in memory to hold dynamic byffer that is alignment and holds MAX_OBJECTS
            // this->modelTransferSpace =
            //     (UboModel*)aligned_alloc(this->uniformAlignment, this->uniformAlignment * MAX_OBJECTS);
            return aligned_alloc(this->uniformAlignment, this->uniformAlignment * max);
        }

        template <typename T>
        void updateUniformBuffers(uint32_t imageIndex, const std::vector<T>& meshList) {

            // Copy Model data
            void* data = nullptr;
            // for (size_t i = 0; i < meshList.size(); i++) {

            //     std::byte* ptr_base = reinterpret_cast<std::byte*>(this->modelTransferSpace);
            //     std::byte* ptr_atual = ptr_base + (i * this->uniformAlignment);
            //     UboModel* thisModel = std::launder(reinterpret_cast<UboModel*>(ptr_atual));

            //     *thisModel = meshList[i].getModel(); // FIXME: modelo antigo funcionava, reavaliar
            // }

            // Map the list of model data // FIXME: usar class Buffer abaixo!!
            vkMapMemory(logical, this->memory[imageIndex], 0, this->uniformAlignment * meshList.size(), 0, &data);
            memory(data, this->modelTransferSpace, this->uniformAlignment * meshList.size());
            vkUnmapMemory(logical, this->memory[imageIndex]);
        }

      private:
        VkDevice logical;
        VkDeviceSize minOffset;
        size_t uniformAlignment;
        void* modelTransferSpace;
        std::vector<VkBuffer> modelDUniformBuffer;
        std::vector<VkDeviceMemory> memory; // modelDUniformBufferMemory
    };
} // namespace ce
