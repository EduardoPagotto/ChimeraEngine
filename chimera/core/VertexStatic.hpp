#ifndef __CHIMERA_VERTEX_STATIC_HPP
#define __CHIMERA_VERTEX_STATIC_HPP

#include "VertexData.hpp"
#include <vector>

namespace Chimera {

class VertexRenderStatic {
  public:
    VertexRenderStatic();
    virtual ~VertexRenderStatic();
    void create(std::vector<VertexData>& vertexDataIn, std::vector<unsigned int> index);
    void render();

  private:
    unsigned int VAO;
    unsigned int VertexVBOID;
    unsigned int IndexVBOID;
    std::vector<unsigned int> indexIBO;
    std::vector<VertexData> vertexData;
};

// void indexVBO_slow(std::vector<VertexData>& inData, std::vector<VertexData>& outData,
//                    std::vector<unsigned int>& out_indices);

} // namespace Chimera
#endif