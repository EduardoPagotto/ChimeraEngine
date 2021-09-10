#ifndef __CHIMERA_VERTEX_STATIC_HPP
#define __CHIMERA_VERTEX_STATIC_HPP

#include "chimera/core/EBO.hpp"
#include "chimera/core/VAO.hpp"
#include "chimera/core/VBO.hpp"

namespace Chimera {

class VertexRenderStatic {
  public:
    VertexRenderStatic(std::vector<VertexData>& vertexDataIn, std::vector<unsigned int> index);
    virtual ~VertexRenderStatic();
    void render();

  private:
    VAO* vao;
    EBO* ebo;
    std::vector<unsigned int> indexIBO;
    std::vector<VertexData> vertexData;
};
} // namespace Chimera
#endif