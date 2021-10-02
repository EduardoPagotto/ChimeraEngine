#ifndef __CHIMERA_CORE_BUFFER__HPP
#define __CHIMERA_CORE_BUFFER__HPP

#include "chimera/core/VertexData.hpp"

namespace Chimera::Core {

enum class BufferType // BufferUsage
{
    STATIC,
    DYNAMIC,
    STREAM
};

class VertexBuffer {
  public:
    VertexBuffer(BufferType type);
    virtual ~VertexBuffer();
    void reSize(const uint32_t& size);
    void setLayout(VertexComponents* layout);
    void setData(const void* data, const uint32_t& size);

    void* map();
    void unmap();
    void bind() const;
    void unbind() const;
    // se nao existir IBO new rander pelo VAO
    // inline void render() const { glDrawArrays(GL_TRIANGLES, 0, this->size); }
  private:
    uint32_t bufferID;
    VertexComponents* layout;
    BufferType type;
};
} // namespace Chimera::Core
#endif