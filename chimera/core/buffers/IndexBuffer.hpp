#ifndef __CHIMERA_CORE_INDEXBUFFER__HPP
#define __CHIMERA_CORE_INDEXBUFFER__HPP

#include "chimera/OpenGLDefs.hpp"

namespace Chimera::Core {

class IndexBuffer {
  public:
    IndexBuffer(uint32_t* data, const uint32_t& size);
    virtual ~IndexBuffer();
    inline void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID); }
    inline void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
    inline uint32_t getCount() const { return size; }

  private:
    uint32_t bufferID;
    uint32_t size;
};
} // namespace Chimera::Core
#endif