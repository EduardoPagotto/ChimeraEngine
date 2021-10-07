#ifndef __CHIMERA_CORE_INDEXBUFFER__HPP
#define __CHIMERA_CORE_INDEXBUFFER__HPP

#include <cstdint>

namespace Chimera {

class IndexBuffer {
  public:
    IndexBuffer(uint32_t* data, const uint32_t& size);
    virtual ~IndexBuffer();
    void bind() const;
    void unbind() const;
    inline uint32_t getCount() const { return size; }
    inline uint32_t getBufferID() const { return bufferID; }

  private:
    uint32_t bufferID;
    uint32_t size;
};
} // namespace Chimera
#endif