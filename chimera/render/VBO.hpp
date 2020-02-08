#ifndef __CHIMERA_VBO__HPP
#define __CHIMERA_VBO__HPP

#include "chimera/render/VAO.hpp"
#include "chimera/render/VertexData.hpp"
#include <vector>

namespace Chimera {

class VBO {
  public:
    VBO();
    virtual ~VBO();

  protected:
    void initialize(std::vector<VertexData>& vertexDataIn, const int& maxBufferSize);
    virtual void createIndex() = 0;
    virtual void clearIndex() = 0;

    VAO vao;
    unsigned int vbo;
};

} // namespace Chimera
#endif