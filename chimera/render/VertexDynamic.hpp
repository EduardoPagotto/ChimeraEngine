#ifndef __CHIMERA_VERTEX_DYNAMIC_HPP
#define __CHIMERA_VERTEX_DYNAMIC_HPP

#include "VBO.hpp"

namespace Chimera {

class VertexRenderDynamic {
  public:
    VertexRenderDynamic();
    void create(const unsigned int& max);
    virtual ~VertexRenderDynamic();
    void render(std::vector<VertexData>& vVertice);

  private:
    VBO vbo;
};
} // namespace Chimera
#endif