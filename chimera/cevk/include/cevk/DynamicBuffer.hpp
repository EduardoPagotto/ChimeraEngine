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
            this->uniform_alignment_ = (size + this->min_offset_ - 1) & ~(this->min_offset_ - 1);

            // Create space in memory to hold dynamic byffer that is alignment and holds max_objects
            // this->modelTransferSpace =
            //     (UboModel*)aligned_alloc(this->uniformAlignment, this->uniformAlignment * max_objects);
            return aligned_alloc(this->uniform_alignment_, this->uniform_alignment_ * max);
        }

        template <typename T>
        void update_uniform_buffers(uint32_t image_index, const std::vector<T>& mesh_list) {

            // Copy Model data
            void* data = nullptr;
            // for (size_t i = 0; i < meshList.size(); i++) {

            //     std::byte* ptr_base = reinterpret_cast<std::byte*>(this->modelTransferSpace);
            //     std::byte* ptr_atual = ptr_base + (i * this->uniformAlignment);
            //     UboModel* thisModel = std::launder(reinterpret_cast<UboModel*>(ptr_atual));

            //     *thisModel = meshList[i].getModel(); // FIXME: modelo antigo funcionava, reavaliar
            // }

            // Map the list of model data // FIXME: usar class Buffer abaixo!!
            vkMapMemory(logical_, this->memory_[image_index], 0, this->uniform_alignment_ * mesh_list.size(), 0, &data);
            memory_(data, this->model_transfer_space_, this->uniform_alignment_ * mesh_list.size());
            vkUnmapMemory(logical_, this->memory_[image_index]);
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
