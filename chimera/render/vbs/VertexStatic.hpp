#ifndef __CHIMERA_VERTEX_STATIC_HPP
#define __CHIMERA_VERTEX_STATIC_HPP

#include "chimera/core/buffers/IndexBuffer.hpp"
#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/core/buffers/VertexBuffer.hpp"

namespace Chimera {

class VertexRenderStatic {
  public:
    VertexRenderStatic(std::vector<VertexData>& vertexDataIn, std::vector<unsigned int> index);
    virtual ~VertexRenderStatic();
    void render();

  private:
    Core::VertexArray* vao;
    Core::IndexBuffer* ebo;
    std::vector<unsigned int> indexIBO;
    std::vector<VertexData> vertexData;
};
} // namespace Chimera
#endif