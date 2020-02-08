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

    VAO vao;
    void buildDynamic(const int& maxBufferSize);
    void buildStatic(std::vector<VertexData>& vertexData);

  protected:
    virtual void createIndex(){};
    virtual void clearIndex(){};
    unsigned int vboGL;

  private:
    void setSlot(const unsigned int& slotID, const unsigned int& slotSize, void* offset);
};

} // namespace Chimera
#endif