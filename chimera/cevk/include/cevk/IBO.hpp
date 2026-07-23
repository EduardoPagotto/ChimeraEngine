#pragma once

#include "Buffers.hpp"
#include <vector>

namespace ce {
    class IBO {
      public:
        explicit IBO(VkPhysicalDevice physical, VkDevice logical);
        ~IBO();

        IBO(const IBO&) = delete;
        IBO& operator=(const IBO&) = delete;

        size_t getCount() const { return this->count; }
        VkBuffer get() const { return this->ibo.get(); }

        void destroy();
        void create(VkQueue queue, VkCommandPool commandBuffer, std::vector<uint32_t>* indices);

      private:
        size_t count;
        VkPhysicalDevice physical;
        VkDevice logical;
        Buffer ibo;
    };
} // namespace ce
