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
    void bind();
    void unbind();
    void buildDynamic(const int& maxBufferSize, VAO& vao);
    void buildStatic(std::vector<VertexData>& vertexData, VAO& vao);

    // VAO vao;

  private:
    unsigned int vboGL;
    void setSlot(const unsigned int& slotID, const unsigned int& slotSize, void* offset);
};
} // namespace Chimera
#endif