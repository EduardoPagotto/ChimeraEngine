#ifndef __CHIMERA_CORE_BUFFER__HPP
#define __CHIMERA_CORE_BUFFER__HPP

#include "chimera/OpenGLDefs.hpp"
#include "chimera/core/VertexData.hpp"

namespace Chimera::Core {

class VertexBuffer {
  public:
    VertexBuffer(void* vertexData, const uint32_t& size, VertexComponents* components);
    virtual ~VertexBuffer();
    void enableVertexAttribArray();
    void disableVertexAttribArray();
    inline void bind() const { glBindBuffer(GL_ARRAY_BUFFER, bufferID); }
    inline void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
    inline uint32_t getSize() const { return size; }
    // se nao existir IBO new rander pelo VAO
    // inline void render() const { glDrawArrays(GL_TRIANGLES, 0, this->size); }
  private:
    uint32_t bufferID;
    VertexComponents* components;
    uint32_t size;
};
} // namespace Chimera::Core
#endif