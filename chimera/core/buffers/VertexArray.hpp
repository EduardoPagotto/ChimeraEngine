#ifndef __CHIMERA_CORE_VERTEXARRAY__HPP
#define __CHIMERA_CORE_VERTEXARRAY__HPP

#include "VertexBuffer.hpp"

namespace Chimera::Core {

class VertexArray {
  public:
    VertexArray();
    virtual ~VertexArray();
    void addBuffer(VertexBuffer* buffer, uint32_t index);
    inline void bind() const { glBindVertexArray(arrayID); }
    inline void unbind() const { glBindVertexArray(0); }

  private:
    uint32_t arrayID;
    std::vector<VertexBuffer*> buffers;
};
} // namespace Chimera::Core
#endif