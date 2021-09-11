#ifndef __CHIMERA_CORE_BUFFER__HPP
#define __CHIMERA_CORE_BUFFER__HPP

#include "chimera/OpenGLDefs.hpp"
#include "chimera/core/VertexData.hpp"
#include <vector>

namespace Chimera::Core {

class VertexBuffer {
  public:
    VertexBuffer(VertexData* vertexData, const unsigned int& size, const unsigned int& componentCount);
    virtual ~VertexBuffer();
    void setSlot(const unsigned int& slotID, const unsigned int& slotSize, void* offset);
    inline void bind() const { glBindBuffer(GL_ARRAY_BUFFER, bufferID); }
    inline void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
    inline unsigned int getComponentCount() const { return componentCount; }
    inline void unsetSlot(const unsigned int& slotID) const { glDisableVertexAttribArray(slotID); }

    // se nao existir IBO new rander pelo VAO
    // inline void render() const { glDrawArrays(GL_TRIANGLES, 0, this->size); }

  private:
    unsigned int bufferID;
    unsigned int componentCount;
    unsigned int size;
};
} // namespace Chimera::Core
#endif