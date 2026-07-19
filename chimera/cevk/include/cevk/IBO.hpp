#pragma once

#include "Buffers.hpp"

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
        std::shared_ptr<Buffer> ibo;
    };
} // namespace ce
