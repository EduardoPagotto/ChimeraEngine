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
    RenderBuffer(const uint32_t& posX, const uint32_t& posY, FrameBuffer* fb, const Shader& shader);
    virtual ~RenderBuffer();
    void renderText();
    void bind();
    void unbind();

  private:
    uint32_t posX, posY;
    Shader shader;
    VertexBuffer* vbo;
    FrameBuffer* frameBuffer;
};
} // namespace Chimera
#endif
