#ifndef __BUFFERDYNAMIC_CLASS_HPP
#define __BUFFERDYNAMIC_CLASS_HPP

#include "Mesh.hpp"
#include <cstring>
#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

namespace ce {

    struct UboModel {
        glm::mat4 model;
    };

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

    //         // Map the list of model data // FIXME: usar class BufferObject abaixo!!
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
} // namespace ce
#endif
