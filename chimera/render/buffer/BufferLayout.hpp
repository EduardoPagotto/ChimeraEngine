#pragma once

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
  public:
    BufferLayout() : size(0) {}

    void push(uint16_t count, uint16_t type, uint16_t sizeOfType, bool normalized) {
        layout.push_back({count, type, sizeOfType, normalized, this->size});
        this->size += sizeOfType * count;
    }

    // template <typename T> void Push(uint count = 1, bool normalized = false) {}
    // template <> void Push<glm::vec3>(uint count, bool normalized) { Push(3, GL_FLOAT, sizeof(float),normalized); }

    inline const std::vector<BufferElement>& getLayout() const { return layout; }
    inline uint getStride() const { return size; }

  private:
    uint16_t size;
    std::vector<BufferElement> layout;
};
} // namespace Chimera
