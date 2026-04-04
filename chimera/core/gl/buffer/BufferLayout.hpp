#pragma once
#include "chimera/core/gl/OpenGLDefs.hpp"
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
      private:
        uint16_t size = 0;
        std::vector<BufferElement> layout;

      public:
        BufferLayout() = default;
        virtual ~BufferLayout() = default;

        template <typename T>
        inline void Push(const uint32_t& count, const bool& normalized) {}

        template <>
        inline void Push<float>(const uint32_t& count, const bool& normalized) {
            push(count, GL_FLOAT, sizeof(float), normalized);
        }

        inline const std::vector<BufferElement>& getLayout() const { return layout; }
        inline uint32_t getStride() const { return size; }

      private:
        inline void push(uint16_t count, uint16_t type, uint16_t sizeOfType, bool normalized) {
            layout.push_back({count, type, sizeOfType, normalized, this->size});
            this->size += sizeOfType * count;
        }
    };
} // namespace ce
