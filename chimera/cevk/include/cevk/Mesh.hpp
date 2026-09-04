#pragma once

#include "IBO.hpp"
#include "VBO.hpp"
#include "cevk.hpp"
#include <memory>

namespace ce {

    class Mesh {
      public:
        Mesh() = default;
        Mesh(VkPhysicalDevice physical, VkDevice logical, VkQueue transferQueue, VkCommandPool transferCommandPool,
             std::vector<Vertex>* vertices, std::vector<uint32_t>* indices, int newTexId)
            : texId(newTexId) {
            //
            this->vbo = std::make_shared<VBO>(physical, logical);
            this->vbo->create(transferQueue, transferCommandPool, vertices, sizeof(Vertex));

            this->ibo = std::make_shared<IBO>(physical, logical);
            this->ibo->create(transferQueue, transferCommandPool, indices);
        }

        virtual ~Mesh() = default;

        int getTexId() const { return this->texId; }

        size_t getVertexCount() const { return this->vbo->getCount(); }
        VkBuffer getVertexBuffer() { return this->vbo->getBuffer(); }

        size_t getIndexCount() const { return this->ibo->getCount(); }
        VkBuffer getIndexBuffer() { return this->ibo->get(); }

        void destroyBuffers() {
            this->vbo.reset();
            this->ibo.reset();
        }

      private:
        int texId;

        std::shared_ptr<VBO> vbo;
        std::shared_ptr<IBO> ibo;
    };
} // namespace ce
