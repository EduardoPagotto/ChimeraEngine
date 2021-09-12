#ifndef __CHIMERA_CORE_BUFFER__HPP
#define __CHIMERA_CORE_BUFFER__HPP

#include "chimera/OpenGLDefs.hpp"
#include "chimera/core/VertexData.hpp"

namespace Chimera::Core {

class VertexBuffer {
  public:
    VertexBuffer(VertexData* vertexData, const uint32_t& size, const uint32_t& componentCount);
    virtual ~VertexBuffer();
    void setSlot(const uint32_t& slotID, const uint32_t& slotSize, void* offset);
    inline void bind() const { glBindBuffer(GL_ARRAY_BUFFER, bufferID); }
    inline void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
    inline uint32_t getComponentCount() const { return componentCount; }
    inline void unsetSlot(const uint32_t& slotID) const { glDisableVertexAttribArray(slotID); }

    // se nao existir IBO new rander pelo VAO
    // inline void render() const { glDrawArrays(GL_TRIANGLES, 0, this->size); }

  private:
    uint32_t bufferID;
    uint32_t componentCount;
    uint32_t size;
};
} // namespace Chimera::Core
#endif