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

        size_t get_count() const { return count_; }
        VkBuffer get() const { return buffer_.get(); }

        void destroy();
        void create(VkQueue queue, VkCommandPool command_buffer, std::vector<uint32_t>* indices);

      private:
        size_t count_;
        VkPhysicalDevice physical_;
        VkDevice logical_;
        Buffer buffer_;
    };
} // namespace ce
