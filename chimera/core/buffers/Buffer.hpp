#ifndef __CHIMERA_CORE_BUFFER__HPP
#define __CHIMERA_CORE_BUFFER__HPP

#include "chimera/OpenGLDefs.hpp"
#include "chimera/core/VertexData.hpp"
#include <vector>

namespace Chimera::Core {

class Buffer {
  public:
    Buffer(std::vector<VertexData>* vertexData, const unsigned int& componentCount);
    virtual ~Buffer();
    void setSlot(const unsigned int& slotID, const unsigned int& slotSize, void* offset);
    inline void bind() const { glBindBuffer(GL_ARRAY_BUFFER, bufferID); }
    inline void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
    inline unsigned int getComponentCount() const { return componentCount; }
    inline void unsetSlot(const unsigned int& slotID) const { glDisableVertexAttribArray(slotID); }

  private:
    unsigned int bufferID;
    unsigned int componentCount;
};
} // namespace Chimera::Core
#endif