#ifndef __CHIMERA_CORE_VERTEXARRAY__HPP
#define __CHIMERA_CORE_VERTEXARRAY__HPP

#include "Buffer.hpp"

namespace Chimera::Core {

class VertexArray {
  public:
    VertexArray();
    virtual ~VertexArray();
    void addBuffer(Buffer* buffer, unsigned int index);
    inline void bind() const { glBindVertexArray(arrayID); }
    inline void unbind() const { glBindVertexArray(0); }

  private:
    unsigned int arrayID;
    std::vector<Buffer*> buffers;
};
} // namespace Chimera::Core
#endif