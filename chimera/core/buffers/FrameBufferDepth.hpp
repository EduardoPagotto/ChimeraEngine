#ifndef __CHIMERA_FRAME_BUFFER_DEPTH_HPP
#define __CHIMERA_FRAME_BUFFER_DEPTH_HPP

#include "IFrameBuffer.hpp"
#include "chimera/core/Texture.hpp"
#include <glm/glm.hpp>

namespace Chimera {

class FrameBufferDepth : public IFrameBuffer {
  public:
    FrameBufferDepth(const uint16_t& width, const uint16_t& height);
    virtual ~FrameBufferDepth();

    inline Texture* getTexture() const override { return texture; }
    void bind() const override;
    void unbind() const override;
    void clear() override;

  private:
    uint16_t width, height;
    uint32_t framBufferID;
    glm::vec4 clearColor;
    Texture* texture;
};

} // namespace Chimera
#endif