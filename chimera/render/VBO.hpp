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

  private:
    void setSlot(const unsigned int& slotID, const unsigned int& slotSize, void* offset);
};

} // namespace Chimera
#endif