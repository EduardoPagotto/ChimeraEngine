#ifndef __CHIMERA_VERTEX_DYNAMIC_HPP
#define __CHIMERA_VERTEX_DYNAMIC_HPP

#include "chimera/core/VAO.hpp"
#include "chimera/core/VBO.hpp"

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