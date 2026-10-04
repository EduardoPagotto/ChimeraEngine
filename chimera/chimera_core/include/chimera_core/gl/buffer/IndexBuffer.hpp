#pragma once
#include <cstdint>

namespace ce {

    class IndexBuffer {
      public:
        IndexBuffer(uint32_t* data, const uint32_t& size);
        virtual ~IndexBuffer();
        void bind() const;
        void unbind() const;
        inline const uint32_t get_size() const { return size_; }
        inline const uint32_t get_buffer_id() const { return buffer_id_; }

      private:
        uint32_t buffer_id_ = 0;
        uint32_t size_ = 0;
    };
} // namespace ce
