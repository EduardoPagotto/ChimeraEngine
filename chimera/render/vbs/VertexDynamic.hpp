#ifndef __CHIMERA_VERTEX_DYNAMIC_HPP
#define __CHIMERA_VERTEX_DYNAMIC_HPP

#include "chimera/core/buffers/Buffer.hpp"
#include "chimera/core/buffers/VertexArray.hpp"

namespace Chimera {

class VertexRenderDynamic {
  public:
    VertexRenderDynamic(const unsigned int& max);
    virtual ~VertexRenderDynamic();
    void render(std::vector<VertexData>& vVertice);

  private:
    VAO vao;
    VBO* vbo;
};
} // namespace Chimera
#endif