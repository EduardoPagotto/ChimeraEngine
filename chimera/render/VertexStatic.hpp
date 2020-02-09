#ifndef __CHIMERA_VERTEX_STATIC_HPP
#define __CHIMERA_VERTEX_STATIC_HPP

#include "EBO.hpp"
#include "VAO.hpp"
#include "VBO.hpp"

namespace Chimera {

class VertexRenderStatic {
  public:
    VertexRenderStatic();
    virtual ~VertexRenderStatic();
    void create(std::vector<VertexData>& vertexDataIn, std::vector<unsigned int> index);
    void render();

  private:
    VBO vbo;
    VAO vao;
    EBO ebo;
    std::vector<unsigned int> indexIBO;
    std::vector<VertexData> vertexData;
};
} // namespace Chimera
#endif