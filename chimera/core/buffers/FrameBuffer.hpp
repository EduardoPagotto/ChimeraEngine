#ifndef __CHIMERA_FRAME_BUFFER__HPP
#define __CHIMERA_FRAME_BUFFER__HPP

#include "IFrameBuffer.hpp"
#include "chimera/core/Texture.hpp"
#include <glm/glm.hpp>

namespace Chimera {

struct FrameBufferSpecification {
    FrameBufferSpecification() = default;
    uint32_t width = 800, height = 600;
    std::vector<TexParam> attachments;
    bool swapChainTarget = false;
    int samples = 1;
};

class FrameBufferZ { // TODO: continuar daqui!!!!
  public:
    FrameBufferZ(const FrameBufferSpecification& spec);
    ~FrameBufferZ();

    void bind() const;
    static void unbind();
    void clearAttachment(uint32_t attachmentIndex, const int value);
    void resize(const uint32_t& width, const uint32_t& height);
    int readPixel(uint32_t attachmentIndex, int x, int y);
    inline Texture* getColorAttachemnt(uint32_t index) const { return colorAttachments[index]; } //

    inline Texture* getDepthAttachemnt() { return depthAttachment; }
    void clearDepth(const glm::vec4& value) const;

  private:
    void destroy();
    void invalidade();
    // uint16_t width, height;
    uint32_t framBufferID, rbo;
    FrameBufferSpecification spec;

    Texture* depthAttachment;
    std::vector<Texture*> colorAttachments;
    std::vector<TexParam> colorTexSpecs;
    TexParam rboSpec;
    TexParam depthTexSpec;
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

} // namespace Chimera
#endif