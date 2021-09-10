#ifndef __CHIMERA_VBO__HPP
#define __CHIMERA_VBO__HPP

#include "VertexData.hpp"
#include "chimera/OpenGLDefs.hpp"
#include <vector>

namespace Chimera {

class VBO {
  public:
    VBO(std::vector<VertexData>* vertexData, const unsigned int& componentCount);
    virtual ~VBO();
    inline void bind() const { glBindBuffer(GL_ARRAY_BUFFER, bufferID); }
    inline void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

  private:
    void setSlot(const unsigned int& slotID, const unsigned int& slotSize, void* offset);

    unsigned int bufferID;
    unsigned int componentCount;
};
} // namespace Chimera
#endif