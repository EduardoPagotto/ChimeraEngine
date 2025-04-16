#pragma once
#include "FrameBuffer.hpp"
#include "VertexBuffer.hpp"
#include "chimera/core/gl/Shader.hpp"

namespace ce {

class RenderBuffer {
  private:
    uint32_t posX, posY;
    std::shared_ptr<Shader> shader;
    VertexBuffer* vbo;
    FrameBuffer* frameBuffer;

  public:
    RenderBuffer(const uint32_t& posX, const uint32_t& posY, FrameBuffer* fb, std::shared_ptr<Shader> shader);
    virtual ~RenderBuffer() {
        delete frameBuffer;
        delete vbo;
    }
    void render();
    void bind();
    void unbind() { frameBuffer->unbind(); }
    inline const uint32_t getWidth() const { return frameBuffer->getWidth(); }
    inline const uint32_t getHeight() const { return frameBuffer->getHeight(); }
    inline FrameBuffer* getFramBuffer() const { return frameBuffer; }
};
} // namespace ce
