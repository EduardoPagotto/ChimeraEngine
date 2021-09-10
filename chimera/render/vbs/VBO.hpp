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
    void setSlot(const unsigned int& slotID, const unsigned int& slotSize, void* offset);
    inline void bind() const { glBindBuffer(GL_ARRAY_BUFFER, bufferID); }
    inline void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
    inline unsigned int getComponentCount() const { return componentCount; }
    inline void unsetSlot(const unsigned int& slotID) const { glDisableVertexAttribArray(slotID); }

  private:
    unsigned int bufferID;
    unsigned int componentCount;
};
} // namespace Chimera
#endif