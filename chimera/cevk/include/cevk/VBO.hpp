#pragma once

#include "Buffers.hpp"
#include "cevk.hpp"
#include <memory>

namespace ce {

    class VBO {
      public:
        explicit VBO(VkPhysicalDevice physical, VkDevice logical);
        virtual ~VBO();

        VBO(const VBO&) = delete;
        VBO& operator=(const VBO&) = delete;
        // VBO(VBO&& other) noexcept;
        // VBO& operator=(VBO&& other) noexcept;

        [[nodiscard]] size_t getCount() const { return this->count; }
        [[nodiscard]] VkBuffer getBuffer() const { return this->vbo->getBuffer(); }

        void destroy();
        void create(VkQueue queue, VkCommandPool commandPool, std::vector<Vertex>* vertices, size_t sizeVertex);

      private:
        size_t count;
        VkPhysicalDevice physical;
        VkDevice logical;
        std::shared_ptr<Buffer> vbo;
    };
} // namespace ce
