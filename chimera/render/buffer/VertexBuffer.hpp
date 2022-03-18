#pragma once
#include "chimera/render/OpenGLDefs.hpp"
#include "chimera/render/buffer/BufferLayout.hpp"

namespace Chimera {

#define BUFFER_OFFSET(i) ((void*)(i))

enum class BufferType { STATIC = GL_STATIC_DRAW, DYNAMIC = GL_DYNAMIC_DRAW, STREAM = GL_STREAM_DRAW };

class VertexBuffer {
  public:
    VertexBuffer(BufferType type);
    VertexBuffer(BufferType type, const uint32_t& size, void* data);

    virtual ~VertexBuffer();
    void reSize(const uint32_t& size);
    void setLayout(const BufferLayout& bufferLayout);
    void setData(const void* data, const uint32_t& size);
    void setSubData(const void* data, const uint32_t& offset, const uint32_t& size);

    void* map();
    void unmap();
    void bind() const;
    static void unbind();
    // se nao existir IBO new rander pelo VAO
    // inline void render() const { glDrawArrays(GL_TRIANGLES, 0, this->size); }
  private:
    uint32_t bufferID;
    uint32_t sizeTotal;
    BufferLayout layout;
    BufferType type;
};
} // namespace Chimera
