#ifndef __CHIMERA_VBO__HPP
#define __CHIMERA_VBO__HPP

#include "chimera/render/VAO.hpp"
#include "chimera/render/VertexData.hpp"
#include <vector>

namespace Chimera {

class VBO {
  public:
    VBO(bool _bufferDynamic);
    virtual ~VBO();

  protected:
    void initialize(std::vector<VertexData>& vertexDataIn);
    virtual void createIndex() = 0;
    virtual void clearIndex() = 0;

    VAO vao;
    unsigned int vbo;

    unsigned int maxBufferSize;

  private:
    bool bufferDynamic;
};

} // namespace Chimera
#endif