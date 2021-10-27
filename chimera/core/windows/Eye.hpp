#ifndef __CHIMERA_EYE_HDM__HPP
#define __CHIMERA_EYE_HDM__HPP

#include "chimera/core/Shader.hpp"
#include "chimera/core/buffers/FrameBuffer.hpp"
#include "chimera/core/buffers/VertexBuffer.hpp"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

namespace Chimera {

class Eye {
  public:
    Eye(const uint32_t& posX, const uint32_t& posY, const uint32_t& width, uint32_t height, const Shader& shader);
    virtual ~Eye();
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
