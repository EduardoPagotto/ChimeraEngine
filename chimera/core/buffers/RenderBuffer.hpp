#ifndef __CHIMERA_RENDER_BUFFER__HPP
#define __CHIMERA_RENDER_BUFFER__HPP

#include "FrameBuffer.hpp"
#include "VertexBuffer.hpp"
#include "chimera/core/Shader.hpp"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

namespace Chimera {

class RenderBuffer {
  public:
    RenderBuffer(const uint32_t& posX, const uint32_t& posY, const uint32_t& width, uint32_t height, const Shader& shader);
    virtual ~RenderBuffer();
    inline float getAspectRatio() const { return (float(width)) / ((float)height); }
    void renderText();
    void bind();
    void unbind();

  private:
    uint32_t posX, posY, width, height;
    Shader shader;
    VertexBuffer* vbo;
    FrameBuffer* frameBuffer;
};
} // namespace Chimera
#endif
