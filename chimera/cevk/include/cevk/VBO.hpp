#pragma once

#include "Buffers.hpp"
#include "cevk.hpp"
#include <vulkan/vulkan_core.h>

namespace ce {

    class VBO {
      public:
        explicit VBO() = default;
        explicit VBO(VkPhysicalDevice physical, VkDevice logical) { init(physical, logical); }
        virtual ~VBO() { destroy(); }

        void init(VkPhysicalDevice physical, VkDevice logical) {
            physical_ = physical;
            logical_ = logical;
        }

        void destroy();

        void create(VkQueue queue, VkCommandPool command_pool, std::vector<Vertex>* vertices, size_t size_vertex);

        size_t get_count() const { return count_; }
        VkBuffer get_buffer() const { return buffer_.get(); }

      private:
        size_t count_;
        VkPhysicalDevice physical_{VK_NULL_HANDLE};
        VkDevice logical_{VK_NULL_HANDLE};
        Buffer buffer_;
    };
} // namespace ce
