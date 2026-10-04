#pragma once
#include "chimera_core/gl/OpenGLDefs.hpp"
#include <cstdint>
#include <vector>

namespace ce {

    struct BufferElement {
        uint16_t count;
        uint16_t type;
        uint16_t sizeOfType;
        bool normalized;
        uint64_t offset;
    };

    class BufferLayout {
      public:
        BufferLayout() = default;
        virtual ~BufferLayout() = default;

        template <typename T>
        inline void push(const uint32_t& count, const bool& normalized) {}

        template <>
        inline void push<float>(const uint32_t& count, const bool& normalized) {
            push(count, GL_FLOAT, sizeof(float), normalized);
        }

        inline const std::vector<BufferElement>& get_layout() const { return layout_; }
        inline uint32_t get_stride() const { return size_; }

      private:
        inline void push(uint16_t count, uint16_t type, uint16_t size_of_type, bool normalized) {
            layout_.push_back({count, type, size_of_type, normalized, this->size_});
            this->size_ += size_of_type * count;
        }

      private:
        uint16_t size_ = 0;
        std::vector<BufferElement> layout_;
    };
} // namespace ce
