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

    // template <typename T> void Push(const uint& count, const uint& size, const bool& normalized) {}
    // template <> void Push<float>(bool normalized) { Push(1, sizeof(float), normalized); }
    // template <typename T> struct type_traits;
    // template <> struct type_traits<int> { static const size_t id = 0; };
    // template <> struct type_traits<std::string> { static const size_t id = 1; };
    // template <> struct type_traits<float> { static const size_t id = 2; };

    inline const std::vector<BufferElement>& getLayout() const { return layout; }
    inline uint getStride() const { return size; }

  private:
    uint16_t size;
    std::vector<BufferElement> layout;
};
} // namespace Chimera
