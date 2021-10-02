#ifndef __CHIMERA_CORE_VERTEXARRAY__HPP
#define __CHIMERA_CORE_VERTEXARRAY__HPP

#include "VertexBuffer.hpp"

namespace Chimera::Core {

class VertexArray {
  public:
    VertexArray();
    virtual ~VertexArray();
    void push(VertexBuffer* buffer);
    void bind() const;
    void unbind() const;
    inline VertexBuffer* getBuffer(const uint32_t& index = 0) const { return vbos[index]; }

  private:
    uint32_t arrayID;
    std::vector<VertexBuffer*> vbos;
};
} // namespace Chimera::Core
#endif