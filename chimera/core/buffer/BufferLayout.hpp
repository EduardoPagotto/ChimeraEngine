#pragma once
#include "chimera/core/OpenGLDefs.hpp"
#include <string>
#include <vector>

namespace Chimera {

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
    inline void Push(const uint& count, const bool& normalized) {}

    template <>
    inline void Push<float>(const uint& count, const bool& normalized) {
        push(count, GL_FLOAT, sizeof(float), normalized);
    }

    inline const std::vector<BufferElement>& getLayout() const { return layout; }
    inline uint getStride() const { return size; }

  private:
    inline void push(uint16_t count, uint16_t type, uint16_t sizeOfType, bool normalized) {
        layout.push_back({count, type, sizeOfType, normalized, this->size});
        this->size += sizeOfType * count;
    }
};
} // namespace Chimera
