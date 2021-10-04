#ifndef __CHIMERA_FRAME_BUFFER__HPP
#define __CHIMERA_FRAME_BUFFER__HPP

#include "chimera/core/Texture.hpp"
#include <glm/glm.hpp>

namespace Chimera {

class IFrameBuffer {
  public:
    virtual Texture* getTexture() const = 0;
    virtual void bind() const = 0;
    virtual void unbind() const = 0;
    virtual void clear() = 0;
};

class FrameBuffer : public IFrameBuffer {
  public:
    FrameBuffer(const uint16_t& width, const uint16_t& height);
    virtual ~FrameBuffer();

    inline Texture* getTexture() const override { return texture; }
    void bind() const override;
    void unbind() const override;
    void clear() override;
    inline void setClearColor(const glm::vec4& color) { clearColor = color; }

  private:
    uint16_t width, height;
    uint32_t framBufferID, depthBufferID;
    Texture* texture;
    glm::vec4 clearColor;
};

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