#ifndef __CHIMERA_VERTEX_STATIC_HPP
#define __CHIMERA_VERTEX_STATIC_HPP

#include "VBO.hpp"

namespace Chimera {

class VertexRenderStatic : public VBO {
  public:
    VertexRenderStatic();
    virtual ~VertexRenderStatic();
    void create(std::vector<VertexData>& vertexDataIn, std::vector<unsigned int> index);
    void render();

  private:
    virtual void createIndex() override;
    virtual void clearIndex() override;

    unsigned int ibo;
    unsigned int sizeBufferIndex;
    std::vector<unsigned int> indexIBO;
    std::vector<VertexData> vertexData;
};

// void indexVBO_slow(std::vector<VertexData>& inData, std::vector<VertexData>& outData,
//                    std::vector<unsigned int>& out_indices);

} // namespace Chimera
#endif