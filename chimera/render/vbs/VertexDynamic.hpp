#ifndef __CHIMERA_VERTEX_DYNAMIC_HPP
#define __CHIMERA_VERTEX_DYNAMIC_HPP

#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/core/buffers/VertexBuffer.hpp"

namespace Chimera {

class VertexRenderDynamic {
  public:
    VertexRenderDynamic(const unsigned int& max);
    virtual ~VertexRenderDynamic();
    void render(std::vector<VertexData>& vVertice);

  private:
    Core::VertexArray vao;
    Core::VertexBuffer* vbo;
};
} // namespace Chimera
#endif