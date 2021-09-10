#ifndef __CHIMERA_CORE_INDEXBUFFER__HPP
#define __CHIMERA_CORE_INDEXBUFFER__HPP

#include "chimera/OpenGLDefs.hpp"
#include <vector>

namespace Chimera::Core {

class IndexBuffer {
  public:
    IndexBuffer(std::vector<uint32_t>& index);
    virtual ~IndexBuffer();
    void render();
    inline void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID); }
    inline void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

  private:
    unsigned int bufferID;
    unsigned int sizeIndex;
};
} // namespace Chimera::Core
#endif