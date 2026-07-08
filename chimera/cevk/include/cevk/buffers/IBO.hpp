#pragma once

#include "buffers/BufferObject.hpp"
#include <memory>
#include <vector>

namespace ce {

    class IBO {
      public:
        explicit IBO(VkPhysicalDevice physical, VkDevice logical);
        ~IBO();

        IBO(const IBO&) = delete;
        IBO& operator=(const IBO&) = delete;
        // IBO(IBO&& other) noexcept;
        // IBO& operator=(IBO&& other) noexcept;

        [[nodiscard]] size_t getCount() const { return this->count; }
        [[nodiscard]] VkBuffer getBuffer() const { return this->ibo->getBuffer(); }

        void destroy();
        void create(VkQueue queue, VkCommandPool commandBuffer, std::vector<uint32_t>* indices);

      private:
        size_t count;
        VkPhysicalDevice physical;
        VkDevice logical;
        std::shared_ptr<ce::BufferObject> ibo;
    };
} // namespace ce
