#pragma once
#include <cstdint>

namespace ce {

class IndexBuffer {
  private:
    uint32_t bufferID = 0;
    uint32_t size = 0;

  public:
    IndexBuffer(uint32_t* data, const uint32_t& size);
    virtual ~IndexBuffer();
    void bind() const;
    void unbind() const;
    inline const uint32_t getSize() const { return size; }
    inline const uint32_t getBufferID() const { return bufferID; }
};
} // namespace ce
