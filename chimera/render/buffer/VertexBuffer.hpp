#pragma once

#include "chimera/render/VertexData.hpp"
#include "chimera/render/buffer/BufferLayout.hpp"

namespace Chimera {

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
    void setLayout(const BufferLayout& bufferLayout);
    void setData(const void* data, const uint32_t& size);

    void* map();
    void unmap();
    void bind() const;
    static void unbind();
    // se nao existir IBO new rander pelo VAO
    // inline void render() const { glDrawArrays(GL_TRIANGLES, 0, this->size); }
  private:
    uint32_t bufferID;
    BufferLayout layout;
    BufferType type;
};
} // namespace Chimera
