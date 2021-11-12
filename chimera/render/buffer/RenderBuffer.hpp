#pragma once
#include "FrameBuffer.hpp"
#include "VertexBuffer.hpp"

namespace Chimera {

class RenderBuffer {
  public:
    RenderBuffer(const uint32_t& posX, const uint32_t& posY, FrameBuffer* fb, const Shader& shader);
    virtual ~RenderBuffer();
    void render();
    void bind();
    void unbind();
    const uint32_t getWidth() const { return frameBuffer->getWidth(); }
    const uint32_t getHeight() const { return frameBuffer->getHeight(); }

  private:
    uint32_t posX, posY;
    Shader shader;
    VertexBuffer* vbo;
    FrameBuffer* frameBuffer;
};
} // namespace Chimera
