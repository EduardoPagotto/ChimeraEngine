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
        explicit BufferDynamic(VkDevice logical) : logical_(logical) {}
        ~BufferDynamic() = default;

      private:
        void* allocate_dynamic_buffer_transfer_space(size_t size, uint32_t max) { // size:=sizeof(UboModel), max_objects

            // Caculate alignment of model data
            uniform_alignment_ = (size + min_offset_ - 1) & ~(min_offset_ - 1);

            // Create space in memory to hold dynamic byffer that is alignment and holds max_objects
            // modelTransferSpace =
            //     (UboModel*)aligned_alloc(uniformAlignment, uniformAlignment * max_objects);
            return aligned_alloc(uniform_alignment_, uniform_alignment_ * max);
        }

        template <typename T>
        void update_uniform_buffers(uint32_t image_index, const std::vector<T>& mesh_list) {

            // Copy Model data
            void* data = nullptr;
            // for (size_t i = 0; i < meshList.size(); i++) {

            //     std::byte* ptr_base = reinterpret_cast<std::byte*>(modelTransferSpace);
            //     std::byte* ptr_atual = ptr_base + (i * uniformAlignment);
            //     UboModel* thisModel = std::launder(reinterpret_cast<UboModel*>(ptr_atual));

            //     *thisModel = meshList[i].getModel(); // FIXME: modelo antigo funcionava, reavaliar
            // }

            // Map the list of model data // FIXME: usar class Buffer abaixo!!
            vkMapMemory(logical_, memory_[image_index], 0, uniform_alignment_ * mesh_list.size(), 0, &data);
            memory_(data, model_transfer_space_, uniform_alignment_ * mesh_list.size());
            vkUnmapMemory(logical_, memory_[image_index]);
        }

      private:
        VkDevice logical_;
        VkDeviceSize min_offset_;
        size_t uniform_alignment_;
        void* model_transfer_space_;
        std::vector<VkBuffer> model_d_uniform_buffer_;
        std::vector<VkDeviceMemory> memory_; // modelDUniformBufferMemory
    };
} // namespace ce
