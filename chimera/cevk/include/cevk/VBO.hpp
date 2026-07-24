#pragma once

#include "Buffers.hpp"
#include "cevk.hpp"
#include <vulkan/vulkan_core.h>

namespace ce {

    class VBO {
      public:
        explicit VBO() = default;
        explicit VBO(VkPhysicalDevice physical, VkDevice logical) { this->init(physical, logical); }
        virtual ~VBO() { this->destroy(); }

        void init(VkPhysicalDevice physical, VkDevice logical) {
            this->physical = physical;
            this->logical = logical;
        }

        void destroy();

        void create(VkQueue queue, VkCommandPool commandPool, std::vector<Vertex>* vertices, size_t sizeVertex);

        size_t getCount() const { return this->count; }
        VkBuffer getBuffer() const { return this->buffer.get(); }

      private:
        size_t count;
        VkPhysicalDevice physical{VK_NULL_HANDLE};
        VkDevice logical{VK_NULL_HANDLE};
        Buffer buffer;
    };
} // namespace ce
