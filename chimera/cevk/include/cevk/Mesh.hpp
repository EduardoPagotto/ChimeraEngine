#pragma once

#include "IBO.hpp"
#include "VBO.hpp"
#include "cevk.hpp"
#include <memory>

namespace ce {

    class Mesh {
      public:
        Mesh() = default;
        Mesh(VkPhysicalDevice physical, VkDevice logical, VkQueue transfer_queue, VkCommandPool transfer_command_pool,
             std::vector<Vertex>* vertices, std::vector<uint32_t>* indices, int new_tex_id)
            : tex_id_(new_tex_id) {
            //
            this->vbo_ = std::make_shared<VBO>(physical, logical);
            this->vbo_->create(transfer_queue, transfer_command_pool, vertices, sizeof(Vertex));

            this->ibo_ = std::make_shared<IBO>(physical, logical);
            this->ibo_->create(transfer_queue, transfer_command_pool, indices);
        }

        virtual ~Mesh() = default;

        int get_tex_id() const { return this->tex_id_; }

        size_t get_vertex_count() const { return this->vbo_->get_count(); }
        VkBuffer get_vertex_buffer() { return this->vbo_->get_buffer(); }

        size_t get_index_count() const { return this->ibo_->get_count(); }
        VkBuffer get_index_buffer() { return this->ibo_->get(); }

        void destroy_buffers() {
            this->vbo_.reset();
            this->ibo_.reset();
        }

      private:
        int tex_id_;
        std::shared_ptr<VBO> vbo_;
        std::shared_ptr<IBO> ibo_;
    };
} // namespace ce
