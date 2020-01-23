#ifndef __CHIMERA_VERTEX_DYNAMIC_HPP
#define __CHIMERA_VERTEX_DYNAMIC_HPP

#include "VertexData.hpp"
#include <vector>

namespace Chimera {

class VertexRenderDynamic {
  public:
    VertexRenderDynamic();
    void create(const unsigned int& max);
    virtual ~VertexRenderDynamic();
    void render(std::vector<VertexData>& vVertice);

  private:
    unsigned int vbo, vao;
};
} // namespace Chimera
#endif