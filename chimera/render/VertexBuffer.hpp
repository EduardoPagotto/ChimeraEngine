#ifndef __CHIMERA_VERTEX_BUFFER__HPP
#define __CHIMERA_VERTEX_BUFFER__HPP

#include "chimera/render/VertexData.hpp"
#include <vector>

namespace Chimera {

class VertexBuffer {
  public:
    VertexBuffer(bool _bufferDynamic);
    virtual ~VertexBuffer();

  protected:
    void initialize(std::vector<VertexData>& vertexDataIn);
    virtual void createIndex() = 0;
    virtual void clearIndex() = 0;
    unsigned int vao;
    unsigned int vbo;

    unsigned int maxBufferSize;

  private:
    bool bufferDynamic;
};

} // namespace Chimera
#endif